//========================================================================
//
// CairoOutputDev.h
//
// Copyright 2003 Glyph & Cog, LLC
// Copyright 2004 Red Hat, INC
//
//========================================================================

#ifndef CAIROOUTPUTDEV_H
#define CAIROOUTPUTDEV_H

#ifdef USE_GCC_PRAGMAS
#pragma interface
#endif

#include "goo/gtypes.h"
#include <cairo-ft.h>
#include "OutputDev.h"
#include "GfxState.h"

class GfxState;
class GfxPath;
class Gfx8BitFont;
struct GfxRGB;
class CairoFontEngine;
class CairoFont;

//------------------------------------------------------------------------

//------------------------------------------------------------------------
// CairoImage
//------------------------------------------------------------------------
class CairoImage {
public:
  // Constructor.
  CairoImage (double x1, double y1, double x2, double y2);

  // Destructor.
  ~CairoImage ();

  // Set the image cairo surface
  void setImage (cairo_surface_t *image);
  
  // Get the image cairo surface
  cairo_surface_t *getImage () const { return image; }

  // Get the image rectangle
  void getRect (double *xa1, double *ya1, double *xa2, double *ya2)
	  { *xa1 = x1; *ya1 = y1; *xa2 = x2; *ya2 = y2; }
  
private:
  cairo_surface_t *image;  // image cairo surface
  double x1, y1;          // upper left corner
  double x2, y2;          // lower right corner
};


//------------------------------------------------------------------------
// CairoOutputDev
//------------------------------------------------------------------------

class CairoOutputDev: public OutputDev {
public:

  // Constructor.
  CairoOutputDev();

  // Destructor.
  virtual ~CairoOutputDev();

  //----- get info about output device

  // Does this device use upside-down coordinates?
  // (Upside-down means (0,0) is the top left corner of the page.)
  virtual GBool upsideDown() { return gTrue; }

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar() { return gTrue; }

  // Does this device use beginType3Char/endType3Char?  Otherwise,
  // text in Type 3 fonts will be drawn with drawChar/drawString.
  virtual GBool interpretType3Chars() { return gTrue; }

  //----- initialization and control

  // Start a page.
  virtual void startPage(int pageNum, GfxState *state);

  // End a page.
  virtual void endPage() { }

  //----- link borders
  virtual void drawLink(Link *link, Catalog *catalog);

  //----- save/restore graphics state
  virtual void saveState(GfxState *state);
  virtual void restoreState(GfxState *state);

  //----- update graphics state
  virtual void updateAll(GfxState *state);
  virtual void setDefaultCTM(double *ctm);
  virtual void updateCTM(GfxState *state, double m11, double m12,
			 double m21, double m22, double m31, double m32);
  virtual void updateLineDash(GfxState *state);
  virtual void updateFlatness(GfxState *state);
  virtual void updateLineJoin(GfxState *state);
  virtual void updateLineCap(GfxState *state);
  virtual void updateMiterLimit(GfxState *state);
  virtual void updateLineWidth(GfxState *state);
  virtual void updateFillColor(GfxState *state);
  virtual void updateStrokeColor(GfxState *state);
  virtual void updateFillOpacity(GfxState *state);
  virtual void updateStrokeOpacity(GfxState *state);

  //----- update text state
  virtual void updateFont(GfxState *state);

  //----- path painting
  virtual void stroke(GfxState *state);
  virtual void fill(GfxState *state);
  virtual void eoFill(GfxState *state);

  //----- path clipping
  virtual void clip(GfxState *state);
  virtual void eoClip(GfxState *state);

  //----- text drawing
  void beginString(GfxState *state, GooString *s);
  void endString(GfxState *state);
  void drawChar(GfxState *state, double x, double y,
		double dx, double dy,
		double originX, double originY,
		CharCode code, int nBytes, Unicode *u, int uLen);

  virtual GBool beginType3Char(GfxState *state, double x, double y,
			       double dx, double dy,
			       CharCode code, Unicode *u, int uLen);
  virtual void endType3Char(GfxState *state);
  virtual void endTextObject(GfxState *state);

  //----- image drawing
  virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
			     int width, int height, GBool invert,
			     GBool inlineImg);
  void drawImageMaskPrescaled(GfxState *state, Object *ref, Stream *str,
			     int width, int height, GBool invert,
			     GBool inlineImg);
  void drawImageMaskRegular(GfxState *state, Object *ref, Stream *str,
			     int width, int height, GBool invert,
			     GBool inlineImg);

  virtual void drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 int *maskColors, GBool inlineImg);
  virtual void drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				int width, int height,
				GfxImageColorMap *colorMap,
				Stream *maskStr,
				int maskWidth, int maskHeight,
				GfxImageColorMap *maskColorMap);

  virtual void drawMaskedImage(GfxState *state, Object *ref, Stream *str,
				int width, int height,
				GfxImageColorMap *colorMap,
				Stream *maskStr,
				int maskWidth, int maskHeight,
				GBool maskInvert);


  //----- transparency groups and soft masks
  virtual void beginTransparencyGroup(GfxState * /*state*/, double * /*bbox*/,
                                      GfxColorSpace * /*blendingColorSpace*/,
                                      GBool /*isolated*/, GBool /*knockout*/,
                                      GBool /*forSoftMask*/);
  virtual void endTransparencyGroup(GfxState * /*state*/);
  void popTransparencyGroup();
  virtual void paintTransparencyGroup(GfxState * /*state*/, double * /*bbox*/);
  virtual void setSoftMask(GfxState * /*state*/, double * /*bbox*/, GBool /*alpha*/,
                           Function * /*transferFunc*/, GfxColor * /*backdropColor*/);
  virtual void clearSoftMask(GfxState * /*state*/);

  //----- Type 3 font operators
  virtual void type3D0(GfxState *state, double wx, double wy);
  virtual void type3D1(GfxState *state, double wx, double wy,
      double llx, double lly, double urx, double ury);

  //----- special access

  // Called to indicate that a new PDF document has been loaded.
  void startDoc(XRef *xrefA);
 
  GBool isReverseVideo() { return gFalse; }
  
  void setCairo (cairo_t *cr);
  void setPrinting (GBool printing) { this->printing = printing; }

protected:
  void doPath(cairo_t *cairo, GfxState *state, GfxPath *path);
  
  GfxRGB fill_color, stroke_color;
  cairo_pattern_t *fill_pattern, *stroke_pattern;
  double fill_opacity;
  double stroke_opacity;
  CairoFont *currentFont;
  
  XRef *xref;			// xref table for current document

  FT_Library ft_lib;
  CairoFontEngine *fontEngine;
  cairo_t *cairo;
  cairo_matrix_t orig_matrix;
  GBool needFontUpdate;                // set when the font needs to be updated
  GBool printing;
  cairo_surface_t *surface;
  cairo_glyph_t *glyphs;
  int glyphCount;
  cairo_path_t *textClipPath;

  GBool prescaleImages;

  cairo_pattern_t *group;
  cairo_pattern_t *shape;
  cairo_pattern_t *mask;
  cairo_surface_t *cairo_shape_surface;
  cairo_t *cairo_shape;
  int knockoutCount;
  struct ColorSpaceStack {
    GBool knockout;
    GfxColorSpace *cs;
    struct ColorSpaceStack *next;
  } * groupColorSpaceStack;
};

//------------------------------------------------------------------------
// CairoImageOutputDev
//------------------------------------------------------------------------

//XXX: this should ideally not inherit from CairoOutputDev but use it instead perhaps
class CairoImageOutputDev: public CairoOutputDev {
public:

  // Constructor.
  CairoImageOutputDev();

  // Destructor.
  virtual ~CairoImageOutputDev();

  //----- get info about output device

  // Does this device use upside-down coordinates?
  // (Upside-down means (0,0) is the top left corner of the page.)
  virtual GBool upsideDown() { return gTrue; }

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar() { return gFalse; }

  // Does this device use beginType3Char/endType3Char?  Otherwise,
  // text in Type 3 fonts will be drawn with drawChar/drawString.
  virtual GBool interpretType3Chars() { return gFalse; }

  // Does this device need non-text content?
  virtual GBool needNonText() { return gTrue; }

    //----- link borders
  virtual void drawLink(Link *link, Catalog *catalog) { }

  //----- save/restore graphics state
  virtual void saveState(GfxState *state) { }
  virtual void restoreState(GfxState *state) { }

  //----- update graphics state
  virtual void updateAll(GfxState *state) { }
  virtual void setDefaultCTM(double *ctm) { }
  virtual void updateCTM(GfxState *state, double m11, double m12,
				 double m21, double m22, double m31, double m32) { }
  virtual void updateLineDash(GfxState *state) { }
  virtual void updateFlatness(GfxState *state) { }
  virtual void updateLineJoin(GfxState *state) { }
  virtual void updateLineCap(GfxState *state) { }
  virtual void updateMiterLimit(GfxState *state) { }
  virtual void updateLineWidth(GfxState *state) { }
  virtual void updateFillColor(GfxState *state) { }
  virtual void updateStrokeColor(GfxState *state) { }
  virtual void updateFillOpacity(GfxState *state) { }
  virtual void updateStrokeOpacity(GfxState *state) { }

  //----- update text state
  virtual void updateFont(GfxState *state) { }

  //----- path painting
  virtual void stroke(GfxState *state) { }
  virtual void fill(GfxState *state) { }
  virtual void eoFill(GfxState *state) { }

  //----- path clipping
  virtual void clip(GfxState *state) { }
  virtual void eoClip(GfxState *state) { }

  //----- image drawing
  virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
			     int width, int height, GBool invert,
			     GBool inlineImg);
  virtual void drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 int *maskColors, GBool inlineImg);
  virtual void drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height,
				   GfxImageColorMap *colorMap,
				   Stream *maskStr,
				   int maskWidth, int maskHeight,
				   GfxImageColorMap *maskColorMap);
  virtual void drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			       int width, int height,
			       GfxImageColorMap *colorMap,
			       Stream *maskStr,
			       int maskWidth, int maskHeight,
			       GBool maskInvert);

  //----- transparency groups and soft masks
  virtual void beginTransparencyGroup(GfxState * /*state*/, double * /*bbox*/,
				      GfxColorSpace * /*blendingColorSpace*/,
				      GBool /*isolated*/, GBool /*knockout*/,
				      GBool /*forSoftMask*/) {}
  virtual void endTransparencyGroup(GfxState * /*state*/) {}
  virtual void paintTransparencyGroup(GfxState * /*state*/, double * /*bbox*/) {}
  virtual void setSoftMask(GfxState * /*state*/, double * /*bbox*/, GBool /*alpha*/,
			   Function * /*transferFunc*/, GfxColor * /*backdropColor*/) {}
  virtual void clearSoftMask(GfxState * /*state*/) {}

  //----- Image list
  // By default images are not rendred
  void setImageDrawDecideCbk(GBool (*cbk)(int img_id, void *data),
			     void *data) { imgDrawCbk = cbk; imgDrawCbkData = data; }
  // Iterate through list of images.
  int getNumImages() const { return numImages; }
  CairoImage *getImage(int i) const { return images[i]; }

private:
  void saveImage(CairoImage *image);
  
  CairoImage **images;
  int numImages;
  int size;
  GBool (*imgDrawCbk)(int img_id, void *data);
  void *imgDrawCbkData;
};

#endif
