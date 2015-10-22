#-------------------------------------------------
#
# Project created by QtCreator 2015-10-02T19:25:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PSDTool
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        dialog.cpp \
    PsdRead/psdread.cpp \
    PsdWrite/float_base.cpp \
    PsdWrite/MutableData.cpp \
    PsdWrite/PSDPreviewGenerator.cpp \
    PsdWrite/SFPSDBevelEffectLayerInformation.cpp \
    PsdWrite/SFPSDDropShadowEffectLayerInformation.cpp \
    PsdWrite/SFPSDEffectsLayerEffectSignatures.cpp \
    PsdWrite/SFPSDGroupClosingLayer.cpp \
    PsdWrite/SFPSDGroupLayer.cpp \
    PsdWrite/SFPSDGroupOpeningLayer.cpp \
    PsdWrite/SFPSDInnerGlowEffectLayerInformation.cpp \
    PsdWrite/SFPSDInnerShadowEffectLayerInformation.cpp \
    PsdWrite/SFPSDLayer.cpp \
    PsdWrite/SFPSDLayerBlendModes.cpp \
    PsdWrite/SFPSDOuterGlowEffectLayerInformation.cpp \
    PsdWrite/SFPSDSolidFillEffectLayerInformation.cpp \
    PsdWrite/SFPSDWriter.cpp \
    libpsd/adjustment.c \
    libpsd/bevel_emboss.c \
    libpsd/bitmap.c \
    libpsd/blend.c \
    libpsd/boundary.c \
    libpsd/brightness_contrast.c \
    libpsd/channel_image.c \
    libpsd/channel_mixer.c \
    libpsd/color.c \
    libpsd/color_balance.c \
    libpsd/color_mode.c \
    libpsd/color_overlay.c \
    libpsd/curves.c \
    libpsd/descriptor.c \
    libpsd/drop_shadow.c \
    libpsd/effects.c \
    libpsd/file_header.c \
    libpsd/fixed.c \
    libpsd/gaussian_blur.c \
    libpsd/gradient_blend.c \
    libpsd/gradient_fill.c \
    libpsd/gradient_map.c \
    libpsd/gradient_overlay.c \
    libpsd/hue_saturation.c \
    libpsd/image_data.c \
    libpsd/image_resource.c \
    libpsd/inner_glow.c \
    libpsd/inner_shadow.c \
    libpsd/invert.c \
    libpsd/layer_mask.c \
    libpsd/levels.c \
    libpsd/outer_glow.c \
    libpsd/path.c \
    libpsd/pattern.c \
    libpsd/pattern_fill.c \
    libpsd/pattern_overlay.c \
    libpsd/photo_filter.c \
    libpsd/posterize.c \
    libpsd/psd.c \
    libpsd/psd_system.c \
    libpsd/psd_zip.c \
    libpsd/rect.c \
    libpsd/satin.c \
    libpsd/selective_color.c \
    libpsd/solid_color.c \
    libpsd/stream.c \
    libpsd/stroke.c \
    libpsd/threshold.c \
    libpsd/thumbnail.c \
    libpsd/type_tool.c \
    zlib/adler32.c \
    zlib/compress.c \
    zlib/crc32.c \
    zlib/deflate.c \
    zlib/example.c \
    zlib/gzclose.c \
    zlib/gzlib.c \
    zlib/gzread.c \
    zlib/gzwrite.c \
    zlib/infback.c \
    zlib/inffast.c \
    zlib/inflate.c \
    zlib/inftrees.c \
    zlib/minigzip.c \
    zlib/trees.c \
    zlib/uncompr.c \
    zlib/zutil.c \
    libpng/example.c \
    libpng/png.c \
    libpng/pngerror.c \
    libpng/pngget.c \
    libpng/pngmem.c \
    libpng/pngpread.c \
    libpng/pngread.c \
    libpng/pngrio.c \
    libpng/pngrtran.c \
    libpng/pngrutil.c \
    libpng/pngset.c \
    libpng/pngtest.c \
    libpng/pngtrans.c \
    libpng/pngwio.c \
    libpng/pngwrite.c \
    libpng/pngwtran.c \
    libpng/pngwutil.c \
    SimplePsd/SimplePSD.cpp \
    PsdWrite/PSDSmartPtr.cpp

HEADERS  += dialog.h \
    libpsd/libpsd.h \
    libpsd/psd_bitmap.h \
    libpsd/psd_blend.h \
    libpsd/psd_color.h \
    libpsd/psd_config.h \
    libpsd/psd_descriptor.h \
    libpsd/psd_fixed.h \
    libpsd/psd_gradient.h \
    libpsd/psd_math.h \
    libpsd/psd_rect.h \
    libpsd/psd_stream.h \
    libpsd/psd_system.h \
    libpsd/psd_types.h \
    PsdRead/psdread.h \
    PsdWrite/float_base.h \
    PsdWrite/int_base.h \
    PsdWrite/MutableData.h \
    PsdWrite/PSDPreviewGenerator.h \
    PsdWrite/sfloat4.h \
    PsdWrite/SFPSDBevelEffectLayerInformation.h \
    PsdWrite/SFPSDDropShadowEffectLayerInformation.h \
    PsdWrite/SFPSDEffectLayerInformations.h \
    PsdWrite/SFPSDEffectsLayerEffectSignatures.h \
    PsdWrite/SFPSDGroupClosingLayer.h \
    PsdWrite/SFPSDGroupLayer.h \
    PsdWrite/SFPSDGroupOpeningLayer.h \
    PsdWrite/SFPSDInnerGlowEffectLayerInformation.h \
    PsdWrite/SFPSDInnerShadowEffectLayerInformation.h \
    PsdWrite/SFPSDLayer.h \
    PsdWrite/SFPSDLayerBlendModes.h \
    PsdWrite/SFPSDOuterGlowEffectLayerInformation.h \
    PsdWrite/SFPSDSolidFillEffectLayerInformation.h \
    PsdWrite/SFPSDWriter.h \
    PsdWrite/sint4.h \
    zlib/crc32.h \
    zlib/deflate.h \
    zlib/gzguts.h \
    zlib/inffast.h \
    zlib/inffixed.h \
    zlib/inflate.h \
    zlib/inftrees.h \
    zlib/trees.h \
    zlib/zconf.h \
    zlib/zlib.h \
    zlib/zutil.h \
    libpng/png.h \
    libpng/pngconf.h \
    libpng/pngdebug.h \
    libpng/pnginfo.h \
    libpng/pnglibconf.h \
    libpng/pngpriv.h \
    libpng/pngstruct.h \
    CImg/CImg.h \
    SimplePsd/SimplePSD.h \
    PsdWrite/PSDSmartPtr.h

INCLUDEPATH += libpsd \
    libpng \
    PsdRead \
    PsdWrite \
    zlib \
    CImg \
    SimplePsd


FORMS    += dialog.ui

RC_FILE += ico.rc
