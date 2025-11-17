export const LoadingSpinner = () => (
  <div className="flex min-h-screen items-center justify-center bg-gray-50 dark:bg-gray-950">
    <div className="flex flex-col items-center space-y-4">
      <div className="relative">
        <div className="h-16 w-16 animate-spin rounded-full border-4 border-gray-200 dark:border-gray-800" />
        <div className="absolute inset-0 h-16 w-16 animate-spin rounded-full border-4 border-transparent border-t-cyan-500 border-r-blue-500" />
      </div>
      <p className="bg-gradient-to-r from-cyan-600 to-blue-600 bg-clip-text text-sm font-medium text-transparent dark:from-cyan-400 dark:to-blue-400">
        Loading...
      </p>
    </div>
  </div>
);
