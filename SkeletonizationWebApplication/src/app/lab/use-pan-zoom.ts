"use client";

import { useRef, useState } from "react";

const clamp = (value: number, min: number, max: number) => Math.min(max, Math.max(min, value));

const zoomLevels = [0.5, 0.75, 1, 1.25, 1.5, 2, 2.5, 3] as const;

const getClosestZoomIndex = (value: number) => {
  let closestIndex = 0;
  let closestDistance = Number.POSITIVE_INFINITY;

  for (let index = 0; index < zoomLevels.length; index += 1) {
    const distance = Math.abs(zoomLevels[index] - value);
    if (distance < closestDistance) {
      closestDistance = distance;
      closestIndex = index;
    }
  }

  return closestIndex;
};

export const usePanZoom = () => {
  const [zoom, setZoom] = useState(1);
  const [panX, setPanX] = useState(0);
  const [panY, setPanY] = useState(0);
  const [isDragging, setIsDragging] = useState(false);

  const dragState = useRef<{
    dragging: boolean;
    pointerId: number | null;
    startX: number;
    startY: number;
    startPanX: number;
    startPanY: number;
  }>({
    dragging: false,
    pointerId: null,
    startX: 0,
    startY: 0,
    startPanX: 0,
    startPanY: 0
  });

  const zoomIn = () =>
    setZoom((z) => {
      const current = clamp(z, zoomLevels[0], zoomLevels[zoomLevels.length - 1]);
      const currentIndex = getClosestZoomIndex(current);
      return zoomLevels[Math.min(currentIndex + 1, zoomLevels.length - 1)];
    });

  const zoomOut = () =>
    setZoom((z) => {
      const current = clamp(z, zoomLevels[0], zoomLevels[zoomLevels.length - 1]);
      const currentIndex = getClosestZoomIndex(current);
      return zoomLevels[Math.max(currentIndex - 1, 0)];
    });

  const resetView = () => {
    setZoom(1);
    setPanX(0);
    setPanY(0);
  };

  const onWheel: React.WheelEventHandler<HTMLDivElement> = (e) => {
    if (e.deltaY === 0) return;
    e.preventDefault();
    if (e.deltaY < 0) zoomIn();
    else zoomOut();
  };

  const onPointerDown: React.PointerEventHandler<HTMLDivElement> = (e) => {
    if (e.button !== 0) return;
    dragState.current.dragging = true;
    setIsDragging(true);
    dragState.current.pointerId = e.pointerId;
    dragState.current.startX = e.clientX;
    dragState.current.startY = e.clientY;
    dragState.current.startPanX = panX;
    dragState.current.startPanY = panY;

    (e.currentTarget as HTMLDivElement).setPointerCapture(e.pointerId);
  };

  const onPointerMove: React.PointerEventHandler<HTMLDivElement> = (e) => {
    if (!dragState.current.dragging) return;
    if (dragState.current.pointerId !== e.pointerId) return;

    const dx = e.clientX - dragState.current.startX;
    const dy = e.clientY - dragState.current.startY;

    setPanX(dragState.current.startPanX + dx);
    setPanY(dragState.current.startPanY + dy);
  };

  const stopDragging: React.PointerEventHandler<HTMLDivElement> = (e) => {
    if (!dragState.current.dragging) return;
    if (dragState.current.pointerId !== e.pointerId) return;
    dragState.current.dragging = false;
    dragState.current.pointerId = null;
    setIsDragging(false);
  };

  const stageTransform = `translate(${panX}px, ${panY}px) scale(${zoom})`;

  return {
    zoom,
    panX,
    panY,
    isDragging,
    stageTransform,
    zoomIn,
    zoomOut,
    resetView,
    onWheel,
    onPointerDown,
    onPointerMove,
    stopDragging
  };
};
