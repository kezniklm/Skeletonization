find_program(PNPM_EXECUTABLE pnpm)
find_program(YARN_EXECUTABLE yarn)
find_program(NPM_EXECUTABLE npm)

if(NPM_EXECUTABLE)
  set(PKG_INSTALL_CMD ${NPM_EXECUTABLE} ci)
  set(PKG_BUILD_CMD ${NPM_EXECUTABLE} run build)
elseif(YARN_EXECUTABLE)
  set(PKG_INSTALL_CMD ${YARN_EXECUTABLE} install --frozen-lockfile)
  set(PKG_BUILD_CMD ${YARN_EXECUTABLE} build)
elseif(PNPM_EXECUTABLE)
  set(PKG_INSTALL_CMD ${PNPM_EXECUTABLE} install --frozen-lockfile)
  set(PKG_BUILD_CMD ${PNPM_EXECUTABLE} build)
else()
  message(FATAL_ERROR "[SV] pnpm/yarn/npm not found")
endif()

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/node_modules")
  set(NPM_INSTALL_CMD ${NPM_EXECUTABLE} install --no-audit --no-fund)
else()
  set(NPM_INSTALL_CMD ${NPM_EXECUTABLE} ci --no-audit --no-fund)
endif()

file(GLOB_RECURSE REACT_INPUTS
  "${CMAKE_CURRENT_LIST_DIR}/src/*"
  "${CMAKE_CURRENT_LIST_DIR}/index.html"
  "${CMAKE_CURRENT_LIST_DIR}/package.json"
  "${CMAKE_CURRENT_LIST_DIR}/package-lock.json"
  "${CMAKE_CURRENT_LIST_DIR}/pnpm-lock.yaml"
  "${CMAKE_CURRENT_LIST_DIR}/yarn.lock"
  "${CMAKE_CURRENT_LIST_DIR}/vite.config.*"
  "${CMAKE_CURRENT_LIST_DIR}/tsconfig*.json"
  "${CMAKE_CURRENT_LIST_DIR}/.babelrc"
  "${CMAKE_CURRENT_LIST_DIR}/.eslintrc*"
)

list(FILTER REACT_INPUTS EXCLUDE REGEX ".*[\\\\/]node_modules[\\\\/].*")
list(FILTER REACT_INPUTS EXCLUDE REGEX ".*[\\\\/]dist[\\\\/].*")
list(FILTER REACT_INPUTS EXCLUDE REGEX ".*[\\\\/]\\.vite[\\\\/].*")

set(VIS_DIST "${CMAKE_CURRENT_LIST_DIR}/dist")
set(VIS_OUT "${CMAKE_BINARY_DIR}/SkeletonizationVisualizer")
set(VIS_STAMP "${VIS_DIST}/.built.stamp")

add_custom_command(
  OUTPUT "${VIS_STAMP}"
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  COMMAND ${CMAKE_COMMAND} -E echo "[SV] installing deps"
  COMMAND ${PKG_INSTALL_CMD}
  COMMAND ${CMAKE_COMMAND} -E echo "[SV] building (vite)"
  COMMAND ${PKG_BUILD_CMD}
  COMMAND ${CMAKE_COMMAND} -E touch "${VIS_STAMP}"
  DEPENDS ${REACT_INPUTS}
  VERBATIM
  COMMENT "Building Skeletonization Visualizer (React)"
)

add_custom_target(SkeletonizationVisualizer_web ALL
  DEPENDS "${VIS_STAMP}"
)

add_custom_command(TARGET SkeletonizationVisualizer_web POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${VIS_OUT}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${VIS_DIST}" "${VIS_OUT}"
  COMMENT "Copying visualization dist -> ${VIS_OUT}"
)

file(TO_CMAKE_PATH "${VIS_OUT}" VIS_OUT_SLASHED)

add_library(SkeletonizationVisualizer INTERFACE)
target_compile_definitions(SkeletonizationVisualizer INTERFACE VISUALIZER_ASSETS_DIR="${VIS_OUT_SLASHED}")
add_dependencies(SkeletonizationVisualizer SkeletonizationVisualizer_web)
