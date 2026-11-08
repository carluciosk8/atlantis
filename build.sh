#!/bin/bash
# ____________________________
#  Atlantis — MSXgl build script
#
#  Invokes the MSXgl Node.js build tool (engine/script/js/build.js) using the
#  Node binary bundled with MSXgl. The MSXgl tree is expected as a sibling of
#  this project (../MSXgl); override with the MSXGL_PATH environment variable.
#─────────────────────────────────────────────────────────────────────────────

# Resolve this script's directory and run the build from there: the build tool
# uses the current working directory as the project directory.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MSXGL_DIR="${MSXGL_PATH:-${SCRIPT_DIR}/../MSXgl}"

if [ ! -x "${MSXGL_DIR}/tools/build/Node/node" ]; then
	echo "MSXgl not found at ${MSXGL_DIR}" >&2
	exit 1
fi

cd "${SCRIPT_DIR}" || exit 1
exec "${MSXGL_DIR}/tools/build/Node/node" "${MSXGL_DIR}/engine/script/js/build.js" "$@"
