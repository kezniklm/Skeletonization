/**
 * @file example-codes.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides built-in OpenCV code snippets for the preprocessing code editor.
 * @description Defines example scripts keyed by operation name so users can quickly insert and run common preprocessing patterns.
 */

/**
 * @brief Maps example identifiers to executable OpenCV.js code snippets.
 * @description Supplies curated code templates used by the custom code tab to demonstrate typical preprocessing operations.
 * @example
 * const code = EXAMPLE_CODES.edge;
 */
export const EXAMPLE_CODES: Record<string, string> = {
  edge: `// Canny Edge Detection
const gray = new cv.Mat();
cv.cvtColor(src, gray, cv.COLOR_RGBA2GRAY);
const edges = new cv.Mat();
cv.Canny(gray, edges, 50, 150);
cv.cvtColor(edges, dst, cv.COLOR_GRAY2RGBA);
gray.delete();
edges.delete();`,
  morph: `// Morphological Operations
const gray = new cv.Mat();
cv.cvtColor(src, gray, cv.COLOR_RGBA2GRAY);
const kernel = cv.Mat.ones(5, 5, cv.CV_8U);
cv.morphologyEx(gray, dst, cv.MORPH_OPEN, kernel);
kernel.delete();
gray.delete();`,
  contour: `// Find Contours
const gray = new cv.Mat();
cv.cvtColor(src, gray, cv.COLOR_RGBA2GRAY);
const contours = new cv.MatVector();
const hierarchy = new cv.Mat();
cv.findContours(gray, contours, hierarchy, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE);
src.copyTo(dst);
const color = new cv.Scalar(0, 255, 0, 255);
cv.drawContours(dst, contours, -1, color, 2);
contours.delete();
hierarchy.delete();
gray.delete();`,
  adaptive: `// Adaptive Threshold
const gray = new cv.Mat();
cv.cvtColor(src, gray, cv.COLOR_RGBA2GRAY);
cv.adaptiveThreshold(gray, dst, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 11, 2);
cv.cvtColor(dst, dst, cv.COLOR_GRAY2RGBA);
gray.delete();`,
  bilateral: `// Bilateral Filter (Edge-preserving smoothing)
const temp = new cv.Mat();
cv.cvtColor(src, temp, cv.COLOR_RGBA2RGB);
cv.bilateralFilter(temp, dst, 9, 75, 75);
cv.cvtColor(dst, dst, cv.COLOR_RGB2RGBA);
temp.delete();`,
  sobel: `// Sobel Edge Detection
const gray = new cv.Mat();
cv.cvtColor(src, gray, cv.COLOR_RGBA2GRAY);
const gradX = new cv.Mat();
const gradY = new cv.Mat();
cv.Sobel(gray, gradX, cv.CV_16S, 1, 0);
cv.Sobel(gray, gradY, cv.CV_16S, 0, 1);
cv.convertScaleAbs(gradX, gradX);
cv.convertScaleAbs(gradY, gradY);
cv.addWeighted(gradX, 0.5, gradY, 0.5, 0, dst);
cv.cvtColor(dst, dst, cv.COLOR_GRAY2RGBA);
gray.delete();
gradX.delete();
gradY.delete();`
};
