CXX_COMPILER     = clang++
BUILD_DIR        = build
BUILD_TYPE      ?= DEBUG
SERVER_PROJ_NAME = server
CLIENT_PROJ_NAME = client

all: run-server

define PrintExec
	@COLOR=$$(tput setaf 123); \
	NC=$$(tput sgr0);          \
	echo "$${COLOR}============< exec ${1}::${BUILD_TYPE} >============$${NC}"
endef

.PHONY:         \
	configure    \
	all          \
	build-server \
	build-client \
	run-server   \
	run-client   \
	clean

configure:
	@cmake                                  \
		-G Ninja                             \
		-DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
		-DBUILD_TYPE=${BUILD_TYPE}           \
		-B ${BUILD_DIR}

#
# build
#
build-server: configure
	@cmake --build ${BUILD_DIR} --target ${SERVER_PROJ_NAME}

build-client: configure
	@cmake --build ${BUILD_DIR} --target ${CLIENT_PROJ_NAME}

#
# run
#
run-server: build-server
	$(call PrintExec,SERVER)
	@./${BUILD_DIR}/${SERVER_PROJ_NAME}

run-client: build-client
	$(call PrintExec,CLIENT)
	@./${BUILD_DIR}/${CLIENT_PROJ_NAME}

#
# clean
#
clean: 
	@rm -rf ${BUILD_DIR}
