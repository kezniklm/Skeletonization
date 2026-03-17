/**
 * @file main.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Boots the benchmark visualizer React application.
 * @description Initializes the root React tree and wraps the app with theme context support.
 * @version 1.0
 * @date 2026-03-16
 */

import { StrictMode } from "react";
import { createRoot } from "react-dom/client";

import "./index.css";
import { ThemeProvider } from "./contexts/ThemeContext";
import SkeletonizerBenchmarkVisualizer from "./pages/SkeletonizerBenchmarkVisualizer";

createRoot(document.getElementById("root")!).render(
  <StrictMode>
    <ThemeProvider>
      <SkeletonizerBenchmarkVisualizer />
    </ThemeProvider>
  </StrictMode>
);
