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
