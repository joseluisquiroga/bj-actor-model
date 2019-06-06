

define SET_g_FLAG
ifeq "${DBG_FLAG}" ""
	g_FLAG := 
else
	g_FLAG := -g3
endif
endef

$(eval $(SET_g_FLAG))

MC_PTD_LIB_DIR := pthreads/lib/

SUBMAKEFILES := \
	mc-ptd-tak-mak-lib.mk \
	mc-ptd-dlmalloc-lib.mk \
	mc-ptd-cell-lib.mk \

