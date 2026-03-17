/**
 * @file images-header.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders image library page heading.
 * @description Provides title and short descriptive subtitle for image management page.
 * @version 1.0
 * @date 2026-03-16
 */

/**
 * @brief Renders image library heading block.
 * @returns Header JSX.
 */
export const ImagesHeader = () => (
  <div className="mb-4 xl:mb-3 2xl:mb-4">
    <h1 className="bg-linear-to-r from-cyan-600 to-blue-600 bg-clip-text pb-0.5 text-4xl font-bold text-transparent xl:text-3xl 2xl:text-4xl dark:from-cyan-400 dark:to-blue-400">
      Image Library
    </h1>
    <p className="text-muted-foreground mt-2 text-sm xl:text-xs 2xl:text-sm">
      Manage your images and prepare them for processing
    </p>
  </div>
);
