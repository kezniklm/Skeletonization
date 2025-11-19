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
