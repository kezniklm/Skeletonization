"use client";

import { useEffect, useRef, type ReactNode } from "react";

import { useAnimatedBackgroundPreference } from "@/contexts/animated-background-preference-context";

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
  dark: {
    nodeGlowOpacity: 0.5,
    nodeCoreOpacity: 0.7,
    nodeColor: { r: 255, g: 255, b: 255 },
    connectionColor: { r: 255, g: 255, b: 255 }
  },
  light: {
    nodeGlowOpacity: 0.35,
    nodeCoreOpacity: 0.5,
    nodeColor: { r: 6, g: 182, b: 212 }, // cyan-500
    connectionColor: { r: 59, g: 130, b: 246 } // blue-500
  }
} as const;

const isDarkMode = (): boolean => document.documentElement.classList.contains("dark");

const observeThemeChanges = (callback: () => void): MutationObserver => {
  const observer = new MutationObserver((mutations) => {
    mutations.forEach((mutation) => {
      if (mutation.attributeName === "class") {
        callback();
      }
    });
  });

  observer.observe(document.documentElement, {
    attributes: true,
    attributeFilter: ["class"]
  });

  return observer;
};

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

const drawConnections = (ctx: CanvasRenderingContext2D, nodes: Node[], dark: boolean): void => {
  const { maxConnectionDistance } = ANIMATION_CONFIG;
  const { connectionLineWidth, maxConnectionOpacity } = VISUAL_CONFIG;
  const color = dark ? VISUAL_CONFIG.dark.connectionColor : VISUAL_CONFIG.light.connectionColor;

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

      gradient.addColorStop(0, `rgba(${color.r},${color.g},${color.b},${opacity})`);
      gradient.addColorStop(1, `rgba(${color.r},${color.g},${color.b},${endOpacity})`);

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
  const config = dark ? VISUAL_CONFIG.dark : VISUAL_CONFIG.light;
  const color = config.nodeColor;

  nodes.forEach((node) => {
    const gradient = ctx.createRadialGradient(node.x, node.y, 0, node.x, node.y, nodeRadius);

    gradient.addColorStop(0, `rgba(${color.r},${color.g},${color.b},${config.nodeGlowOpacity})`);
    gradient.addColorStop(1, `rgba(${color.r},${color.g},${color.b},0)`);

    ctx.fillStyle = gradient;
    ctx.beginPath();
    ctx.arc(node.x, node.y, nodeRadius, 0, Math.PI * 2);
    ctx.fill();

    ctx.fillStyle = `rgba(${color.r},${color.g},${color.b},${config.nodeCoreOpacity})`;
    ctx.beginPath();
    ctx.arc(node.x, node.y, coreRadius, 0, Math.PI * 2);
    ctx.fill();
  });
};

export const AnimatedBackgroundProvider = ({ children }: AnimatedBackgroundProviderProps) => {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const { animatedBackgroundDisabled } = useAnimatedBackgroundPreference();

  useEffect(() => {
    if (animatedBackgroundDisabled) return;

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
      drawConnections(ctx, nodes, dark);
      drawNodes(ctx, nodes, dark);

      animationFrameId = window.requestAnimationFrame(animate);
    };

    animationFrameId = window.requestAnimationFrame(animate);

    // Observe theme changes and trigger re-render
    const themeObserver = observeThemeChanges(() => {
      // Theme changed, canvas will re-render on next animation frame
    });

    return () => {
      window.removeEventListener("resize", resizeCanvas);
      if (animationFrameId !== null) {
        window.cancelAnimationFrame(animationFrameId);
      }
      themeObserver.disconnect();
    };
  }, [animatedBackgroundDisabled]);

  return (
    <>
      {!animatedBackgroundDisabled && (
        <canvas ref={canvasRef} className="pointer-events-none fixed inset-0 z-0" style={{ mixBlendMode: "normal" }} />
      )}
      {children}
    </>
  );
};
