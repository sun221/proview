/*
 * ProviewR   Open Source Process Control.
 * Copyright (C) 2005-2019 SSAB EMEA AB.
 *
 * This file is part of ProviewR.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProviewR. If not, see <http://www.gnu.org/licenses/>
 *
 * Linking ProviewR statically or dynamically with other modules is
 * making a combined work based on ProviewR. Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * In addition, as a special exception, the copyright holders of
 * ProviewR give you permission to, from the build function in the
 * ProviewR Configurator, combine ProviewR with modules generated by the
 * ProviewR PLC Editor to a PLC program, regardless of the license
 * terms of these modules. You may copy and distribute the resulting
 * combined work under the terms of your choice, provided that every
 * copy of the combined work is accompanied by a complete copy of
 * the source code of ProviewR (the version used to produce the
 * combined work), being distributed under the terms of the GNU
 * General Public License plus this exception.
 */

#ifndef glow_draw_qt_h
#define glow_draw_qt_h

#include "co_smart_ptr.h"

#include "glow_customcolors.h"
#include "glow_draw.h"

#include <QColor>
#include <QEvent>
#include <QGradient>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QWidget>

#define DRAW_CLIP_SIZE 10

class GlowCustomColors;

class DrawWindQt : public DrawWind {
public:
  DrawWindQt();
  QWidget* window;
  QRect clip_rectangle[DRAW_CLIP_SIZE];
  QImage* background_pixmap;
};

class GlowCustomColorsQt;

class GlowDrawQt : private QObject, public GlowDraw {
  Q_OBJECT

public:
  GlowDrawQt(QWidget* toplevel, void** glow_ctx,
      int (*init_proc)(QWidget* w, GlowCtx* ctx, void* client_data),
      void* client_data, glow_eCtxType type);

  ~GlowDrawQt();

  DrawWindQt m_wind;
  DrawWindQt nav_wind;
  GlowWind *w = NULL;
  QColor background;
  QColor original_background;
  QTimer* timer_id;
  int click_sensitivity;
  int closing_down;
  GlowCustomColorsQt* customcolors[10];
  int customcolors_cnt;

  void event_handler(QEvent* event, QWidget *target);
  void enable_event(glow_eEvent event, glow_eEventType event_type,
      int (*event_cb)(GlowCtx* ctx, glow_tEvent event));
  void clear();

  int begin(GlowWind *w);
  void end();
  void set_dirty(DrawWind *w);

  void get_window_size(DrawWind* w, int* width, int* height);
  void set_window_size(DrawWind* w, int width, int height);

  void rect(int x, int y, int width, int height, glow_eDrawType painter_type,
      int fill, int idx, int highlight = 0);
  void arrow(int x1, int y1, int x2, int y2, int x3, int y3,
      glow_eDrawType painter_type, int idx, int highlight);
  void arc(int x, int y, int width, int height, int angle1, int angle2,
      glow_eDrawType painter_type, int fill, int idx, int highlight = 0);
  void line(int x1, int y1, int x2, int y2, glow_eDrawType painter_type,
      int idx, int highlight = 0,
      glow_eLineType line_type = glow_eLineType_Solid);
  void polyline(glow_sPointX* points, int point_cnt,
      glow_eDrawType painter_type, int fill, int idx, int highlight = 0);
  void text(int x, int y, char* text, int len, glow_eDrawType painter_type,
      glow_eDrawType color, int idx, int highlight, glow_eFont font_idx,
      double size, int rot);
  void text_cursor(int x, int y, char* text, int len,
      glow_eDrawType painter_type, glow_eDrawType color, int idx, int highlight,
      int pos, glow_eFont font, double size);
  void image(int x, int y, int width, int height, glow_tImImage image,
      glow_tPixmap pixmap, glow_tPixmap clip_mask);
  void set_cursor(DrawWind* w, glow_eDrawCursor cursor);
  void get_text_extent(const char* text, int len, glow_eDrawType painter_type,
      int idx, glow_eFont font_idx, int* width, int* height, int* descent,
      double size, int rot);
  void set_background(DrawWind* w, glow_eDrawType drawtype, glow_tPixmap pixmap,
      glow_tImImage image, int pixmap_width, int pixmap_height);
  void reset_background(DrawWind* w);

  int set_clip_rectangle(DrawWind* w, int ll_x, int ll_y, int ur_x, int ur_y);
  void draw_point(int x1, int y1, glow_eDrawType painter_type);
  void draw_points(glow_sPointX* points, int point_num,
      glow_eDrawType painter_type, int idx = 0);
  void set_click_sensitivity(int value);
  int print(char* filename, double x0, double x1, int end);
  int export_image(char* filename);
  void set_timer(GlowCtx* ctx, int time_ms, void (*callback_func)(GlowCtx* ctx),
      void** id);
  void remove_timer(void* id);
  void init_nav(QWidget* nav_widget);
  unique_ptr<QPoint[]> points_to_qt_points_curve(
      glow_sPointX* points, int point_cnt, int* cnt);
  int image_get_width(glow_tImImage image);
  int image_get_height(glow_tImImage image);
  int image_get_rowstride(glow_tImImage image);
  unsigned char* image_get_data(glow_tImImage image);
  void image_copy(glow_tImImage orig_image, glow_tImImage* image);
  void image_rotate(glow_tImImage* image, int to_rotation, int from_rotation);
  void image_flip_vertical(glow_tImImage* image);
  void image_flip_horizontal(glow_tImImage* image);
  int image_scale(int width, int height, glow_tImImage orig_im,
      glow_tImImage* im, glow_tImData* im_data, glow_tPixmap* im_pixmap,
      glow_tPixmap* im_mask);
  int image_load(char* imagefile, glow_tImImage* orig_im, glow_tImImage* im,
      glow_tImData* im_data);
  void image_free(glow_tImImage image);
  void image_pixel_iter(glow_tImImage orig_image, glow_tImImage* image,
      void (*pixel_cb)(void*, unsigned char*), void* userdata);
  QGradient* gradient_create_pattern(int x, int y, int w, int h,
      glow_eDrawType d0, glow_eDrawType d1, glow_eDrawType d2,
      glow_eGradient gradient);
  void gradient_fill_rect(int x, int y, int w, int h, glow_eDrawType d0,
      glow_eDrawType d1, glow_eDrawType d2, glow_eGradient gradient);
  void gradient_fill_rectrounded(int x, int y, int w, int h, int roundamount,
      glow_eDrawType d0, glow_eDrawType d1, glow_eDrawType d2,
      glow_eGradient gradient);
  void gradient_fill_arc(int x, int y, int w, int h, int angle1, int angle2,
      glow_eDrawType d0, glow_eDrawType d1, glow_eDrawType d2,
      glow_eGradient gradient);
  void gradient_fill_polyline(glow_sPointX* points, int point_cnt,
      glow_eDrawType d0, glow_eDrawType d1, glow_eDrawType d2,
      glow_eGradient gradient);
  void event_exec(void* event, unsigned int size);
  int open_color_selection(double* r, double* g, double* b);
  void update_color(glow_eDrawType color);
  void push_customcolors(GlowCustomColors* cc);
  void set_customcolors(GlowCustomColors* cc);
  void pop_customcolors();
  GlowCustomColors* create_customcolors();
  GlowCustomColorsQt* get_customcolors();
  void reset_customcolors(GlowCustomColors* cc);

private:
  void get_painter(int painter_type, int size);
  void event_timer(QMouseEvent *event, QWidget *target);
  void cancel_event_timer();

  void set_clip();
  void reset_clip();

  void (*draw_timer_callback_func)(GlowCtx* ctx);
  QTimer* draw_timer_id;
  QPainter painter;

public slots:
  bool event_timer_cb();
  bool draw_timer_cb();
};

class DrawPs {
public:
  DrawPs(char* filename) : fp(filename), x(0), y(0) {}

  ~DrawPs()
  {
    fp.close();
  }

  std::ofstream fp;
  double x;
  double y;
};

class GlowCustomColorsQt : public GlowCustomColors {
public:
  GlowCustomColorsQt() : color_vect_cnt(0)
  {
    memset(color_vect, 0, sizeof(color_vect));
  }
  ~GlowCustomColorsQt() {}

  QColor
      color_vect[glow_eDrawType_CustomColor__ - glow_eDrawType_CustomColor1];
  int color_vect_cnt;
};

#endif
