export const APP_CONFIG = {
  DATA_REFRESH_INTERVAL: 2000, // ms
  SLIDESHOW_INTERVAL: 3000, // ms
  DEFAULT_SLIDER_POSITION: 50 // %
} as const;

export const ZOOM_CONFIG = {
  MIN_SCALE: 0.1,
  MAX_SCALE: 10,
  ZOOM_FACTOR: 1.2,
  WHEEL_ZOOM_FACTOR: 0.9,
  DEFAULT_SCALE: 1
} as const;

export const UI_CONSTANTS = {
  BORDER_COLORS: {
    ORIGINAL: "#f59e0b",
    HOVER_SHADOW: "rgba(102,126,234,0.3)"
  },
  ANIMATION_CLASSES: {
    FADE_IN: "fadeIn_0.2s_ease-out",
    SCALE_IN: "scaleIn_0.3s_cubic-bezier(0.4,0,0.2,1)",
    SLIDE_DOWN: "slideDown_0.4s_ease-out",
    FADE_IN_UP: "fadeInUp_0.6s_ease-out"
  },
  MODAL_SIZE: {
    MAX_WIDTH: "95vw",
    MAX_HEIGHT: "95vh"
  }
} as const;

export const KEYBOARD_SHORTCUTS = {
  ESCAPE: "Escape",
  ZOOM_IN: ["+", "="],
  ZOOM_OUT: ["-", "_"],
  RESET_ZOOM: "0",
  FIT_TO_SCREEN: ["f", "F"],
  DOWNLOAD: ["d", "D"],
  ARROW_LEFT: "ArrowLeft",
  ARROW_RIGHT: "ArrowRight"
} as const;

export const THEME_COLORS = {
  PRIMARY: "#667eea",
  SECONDARY: "#764ba2",
  SUCCESS: "#10b981",
  WARNING: "#f59e0b",
  ERROR: "#ef4444",
  GRADIENTS: {
    PRIMARY: "from-purple-500 to-pink-500",
    SUCCESS: "from-green-500 to-emerald-500"
  }
} as const;
