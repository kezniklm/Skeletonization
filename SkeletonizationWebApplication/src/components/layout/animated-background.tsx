"use client";

import { useEffect, useRef, type ReactNode } from "react";

type AnimatedBackgroundProviderProps = {
  children: ReactNode;
};

type Node = {
  x: number;
  y: number;
  vx: number;
  vy: number;
  connections: number[];
};

type NodeDistance = {
  index: number;
  distance: number;
};

const ANIMATION_CONFIG = {
  nodeCount: 96,
  maxConnectionDistance: 300,
  nodeSpeed: 0.3,
  nodeRadius: 4,
  coreRadius: 1.5,
  connectionProbability: 0.15,
  maxConnectionsPerNode: 6,
  canvasMargin: 50,
  minConnectionsPerNode: 2,
  connectionDistanceThreshold: 0.8
} as const;

const VISUAL_CONFIG = {
  connectionLineWidth: 1.5,
  maxConnectionOpacity: 0.3,
  nodeGlowOpacityDark: 0.5,
  nodeGlowOpacityLight: 0.6,
  nodeCoreOpacityDark: 0.7,
  nodeCoreOpacityLight: 0.8
} as const;

const isDarkMode = (): boolean => document.documentElement.classList.contains("dark");

const calculateConnectionProbability = (distance: number, maxDistance: number): number =>
  ANIMATION_CONFIG.connectionProbability * (1 - distance / maxDistance);

const createNodes = (canvas: HTMLCanvasElement): Node[] => {
  const nodes: Node[] = [];
  const { width, height } = canvas;
  const { nodeCount, canvasMargin, nodeSpeed } = ANIMATION_CONFIG;

  for (let i = 0; i < nodeCount; i++) {
    const x = canvasMargin + Math.random() * (width - 2 * canvasMargin);
    const y = canvasMargin + Math.random() * (height - 2 * canvasMargin);

    nodes.push({
      x,
      y,
      vx: (Math.random() - 0.5) * nodeSpeed,
      vy: (Math.random() - 0.5) * nodeSpeed,
      connections: []
    });
  }

  buildNodeConnections(nodes);

  return nodes;
};

const buildNodeConnections = (nodes: Node[]): void => {
  const { maxConnectionDistance, maxConnectionsPerNode, connectionDistanceThreshold } = ANIMATION_CONFIG;
  const threshold = maxConnectionDistance * connectionDistanceThreshold;

  nodes.forEach((node, index) => {
    const nearbyNodes = findNearbyNodes(nodes, node, index, threshold);
    connectNodesToNeighbors(node, nearbyNodes, maxConnectionsPerNode);
    ensureMinimumConnections(node, nearbyNodes);
  });
};

const findNearbyNodes = (nodes: Node[], currentNode: Node, currentIndex: number, maxDistance: number): NodeDistance[] =>
  nodes
    .map((otherNode, otherIndex) => ({
      index: otherIndex,
      distance: Math.hypot(otherNode.x - currentNode.x, otherNode.y - currentNode.y)
    }))
    .filter((d) => d.index > currentIndex && d.distance < maxDistance)
    .sort((a, b) => a.distance - b.distance);

const connectNodesToNeighbors = (node: Node, nearbyNodes: NodeDistance[], maxConnections: number): void => {
  const { maxConnectionDistance, connectionDistanceThreshold } = ANIMATION_CONFIG;
  const threshold = maxConnectionDistance * connectionDistanceThreshold;

  let connectionCount = 0;

  for (const { index: targetIndex, distance } of nearbyNodes) {
    if (connectionCount >= maxConnections) {
      break;
    }

    const probability = calculateConnectionProbability(distance, threshold);

    if (Math.random() < probability) {
      node.connections.push(targetIndex);
      connectionCount++;
    }
  }
};

const ensureMinimumConnections = (node: Node, nearbyNodes: NodeDistance[]): void => {
  const { minConnectionsPerNode } = ANIMATION_CONFIG;

  if (node.connections.length === 0 && nearbyNodes.length > 0) {
    const connectionsToAdd = Math.min(minConnectionsPerNode, nearbyNodes.length);

    for (let i = 0; i < connectionsToAdd; i++) {
      node.connections.push(nearbyNodes[i].index);
    }
  }
};

const updateNodes = (nodes: Node[], canvas: HTMLCanvasElement) => {
  const { width, height } = canvas;

  nodes.forEach((node) => {
    node.x += node.vx;
    node.y += node.vy;

    if (node.x < 0 || node.x > width) {
      node.vx *= -1;
    }

    if (node.y < 0 || node.y > height) {
      node.vy *= -1;
    }

    node.x = Math.max(0, Math.min(width, node.x));
    node.y = Math.max(0, Math.min(height, node.y));
  });
};

const drawConnections = (ctx: CanvasRenderingContext2D, nodes: Node[]): void => {
  const { maxConnectionDistance } = ANIMATION_CONFIG;
  const { connectionLineWidth, maxConnectionOpacity } = VISUAL_CONFIG;

  nodes.forEach((node) => {
    node.connections.forEach((targetIndex) => {
      const target = nodes[targetIndex];

      if (!target) {
        return;
      }

      const dx = target.x - node.x;
      const dy = target.y - node.y;
      const distance = Math.hypot(dx, dy);

      if (distance >= maxConnectionDistance) {
        return;
      }

      const opacity = maxConnectionOpacity * (1 - distance / maxConnectionDistance);
      const gradient = ctx.createLinearGradient(node.x, node.y, target.x, target.y);

      const endOpacity = opacity * 0.5;

      gradient.addColorStop(0, `rgba(255,255,255,${opacity})`);
      gradient.addColorStop(1, `rgba(255,255,255,${endOpacity})`);

      ctx.strokeStyle = gradient;
      ctx.lineWidth = connectionLineWidth;
      ctx.beginPath();
      ctx.moveTo(node.x, node.y);
      ctx.lineTo(target.x, target.y);
      ctx.stroke();
    });
  });
};

const drawNodes = (ctx: CanvasRenderingContext2D, nodes: Node[], dark: boolean): void => {
  const { nodeRadius, coreRadius } = ANIMATION_CONFIG;
  const { nodeGlowOpacityDark, nodeGlowOpacityLight, nodeCoreOpacityDark, nodeCoreOpacityLight } = VISUAL_CONFIG;

  nodes.forEach((node) => {
    const gradient = ctx.createRadialGradient(node.x, node.y, 0, node.x, node.y, nodeRadius);

    const glowOpacity = dark ? nodeGlowOpacityDark : nodeGlowOpacityLight;

    gradient.addColorStop(0, `rgba(255,255,255,${glowOpacity})`);
    gradient.addColorStop(1, "rgba(255,255,255,0)");

    ctx.fillStyle = gradient;
    ctx.beginPath();
    ctx.arc(node.x, node.y, nodeRadius, 0, Math.PI * 2);
    ctx.fill();

    const coreOpacity = dark ? nodeCoreOpacityDark : nodeCoreOpacityLight;

    ctx.fillStyle = `rgba(255,255,255,${coreOpacity})`;
    ctx.beginPath();
    ctx.arc(node.x, node.y, coreRadius, 0, Math.PI * 2);
    ctx.fill();
  });
};

export const AnimatedBackgroundProvider = ({ children }: AnimatedBackgroundProviderProps) => {
  const canvasRef = useRef<HTMLCanvasElement>(null);

  useEffect(() => {
    const canvas = canvasRef.current;

    if (!canvas) {
      return;
    }

    const ctx = canvas.getContext("2d");
    if (!ctx) return;

    let nodes: Node[] = [];
    let animationFrameId: number | null = null;

    const resizeCanvas = () => {
      canvas.width = window.innerWidth;
      canvas.height = window.innerHeight;
      nodes = createNodes(canvas);
    };

    resizeCanvas();

    window.addEventListener("resize", resizeCanvas);

    const animate = () => {
      const dark = isDarkMode();

      ctx.clearRect(0, 0, canvas.width, canvas.height);
      updateNodes(nodes, canvas);
      drawConnections(ctx, nodes);
      drawNodes(ctx, nodes, dark);

      animationFrameId = window.requestAnimationFrame(animate);
    };

    animationFrameId = window.requestAnimationFrame(animate);

    return () => {
      window.removeEventListener("resize", resizeCanvas);
      if (animationFrameId !== null) {
        window.cancelAnimationFrame(animationFrameId);
      }
    };
  }, []);

  return (
    <>
      <canvas ref={canvasRef} className="pointer-events-none fixed inset-0 z-0" style={{ mixBlendMode: "normal" }} />
      {children}
    </>
  );
};
