//#define SK_IGNORE_STDINT_DOT_H

#ifdef _MSC_VER
#pragma comment(lib, "skia_core.lib")
#pragma comment(lib, "skia_effects.lib")
#pragma comment(lib, "skia_images.lib")
#pragma comment(lib, "skia_opts.lib")
#pragma comment(lib, "skia_opts_sse41.lib")
#pragma comment(lib, "skia_opts_ssse3.lib")
#pragma comment(lib, "skia_pdf.lib")
#pragma comment(lib, "skia_ports.lib")
#pragma comment(lib, "skia_sfnt.lib")
#pragma comment(lib, "skia_skgpu.lib")
#pragma comment(lib, "skia_skgputest.lib")
#pragma comment(lib, "skia_svg.lib")
#pragma comment(lib, "skia_utils.lib")
#pragma comment(lib, "skia_xml.lib")
#pragma comment(lib, "crash_handler.lib")
#pragma comment(lib, "experimental.lib")
#pragma comment(lib, "flags.lib")
#pragma comment(lib, "flags_common.lib")
#pragma comment(lib, "jsoncpp.lib")
#pragma comment(lib, "libSkKTX.lib")
#pragma comment(lib, "libetc1.lib")
#pragma comment(lib, "libjpeg.lib")
#pragma comment(lib, "libwebp_dec.lib")
#pragma comment(lib, "libwebp_demux.lib")
#pragma comment(lib, "libwebp_dsp.lib")
#pragma comment(lib, "libwebp_enc.lib")
#pragma comment(lib, "libwebp_utils.lib")
#pragma comment(lib, "lua.lib")
#pragma comment(lib, "picture_utils.lib")
#pragma comment(lib, "proc_stats.lib")
#pragma comment(lib, "resources.lib")
#pragma comment(lib, "sk_tool_utils.lib")
#pragma comment(lib, "skflate.lib")
#pragma comment(lib, "timer.lib")
#endif

#include "SkCanvas.h"
#include "SkData.h"
#include "SkGraphics.h"
#include "SkSurface.h"
#include "SkImage.h"
#include "SkDevice.h"
#include "SkString.h"
#include "SkImageEncoder.h"
#include "SkImageDecoder.h"
#include "SkStream.h"
#include "SkTypeface.h"
#include "SkTextBox.h"
#include "SkForceLinking.h"
#include "SkBlurMaskFilter.h"
