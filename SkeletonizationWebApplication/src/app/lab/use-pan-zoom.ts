/**
 * @file use-pan-zoom.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Implements pan and discrete zoom state for image viewer.
 * @description Handles pointer drag panning, wheel zoom adjustments, and transform generation for viewer stage.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { useRef, useState } from "react";

/**
 * @brief Clamps numeric value to inclusive range.
 * @param value Input numeric value.
 * @param min Minimum allowed value.
 * @param max Maximum allowed value.
 * @returns Clamped numeric value.
 */
const clamp = (value: number, min: number, max: number) => Math.min(max, Math.max(min, value));

/**
 * @brief Defines supported discrete zoom levels.
 */
const zoomLevels = [0.5, 0.75, 1, 1.25, 1.5, 2, 2.5, 3] as const;

/**
 * @brief Resolves nearest zoom-level index to provided value.
 * @param value Current zoom value.
 * @returns Closest index in zoom-level array.
 */
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

/**
 * @brief Provides pan-and-zoom state and interaction handlers.
 * @returns Zoom state, transform, and pointer/wheel handlers.
 */
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
