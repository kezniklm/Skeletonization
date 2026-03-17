/**
 * @file theme-script.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Injects startup script to apply initial theme.
 * @description Resolves configured or system theme before hydration to prevent visual flicker.
 * @version 1.0
 * @date 2026-03-16
 */

/**
 * @brief Renders inline script for initial theme setup.
 * @description Applies `dark` class to document root based on selected theme strategy at page load.
 * @param initialTheme Initial theme mode preference.
 * @returns Inline script element.
 */
export const ThemeScript = ({ initialTheme }: { initialTheme: "light" | "dark" | "system" }) => {
  const themeScript = `
    (function() {
      try {
        const theme = ${JSON.stringify(initialTheme)};
        const getSystemTheme = () => window.matchMedia('(prefers-color-scheme: dark)').matches ? 'dark' : 'light';
        const resolvedTheme = theme === 'system' ? getSystemTheme() : theme;
        
        const root = document.documentElement;
        if (resolvedTheme === 'dark') {
          root.classList.add('dark');
        } else {
          root.classList.remove('dark');
        }
      } catch (e) {}
    })()
  `;

  return <script dangerouslySetInnerHTML={{ __html: themeScript }} />;
};
