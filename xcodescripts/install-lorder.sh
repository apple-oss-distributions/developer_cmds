set -ex

BINDIR="$DSTROOT"/"$DT_TOOLCHAIN_DIR"/usr/bin

install -c \
	"$PROJECT_DIR"/lorder/lorder.sh \
	"$BINDIR"/lorder
