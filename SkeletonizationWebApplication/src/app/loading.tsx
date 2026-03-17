/**
 * @file loading.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides route-level loading fallback UI.
 * @description Displays shared loading spinner while app router segments are resolving.
 * @version 1.0
 * @date 2026-03-16
 */

import { LoadingSpinner } from "@/components/loading/loading-spinner";

/**
 * @brief Renders default loading state for app routes.
 * @returns Loading spinner component.
 */
const Loading = () => <LoadingSpinner />;

export default Loading;
