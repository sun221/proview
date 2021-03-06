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

/* wb_pal_qt.cpp -- Palette of configurator or plc-editor */

#include "cow_qt_helpers.h"

#include "flow_browwidget_qt.h"

#include "wb_pal_qt.h"

#include <QApplication>
#include <QClipboard>
#include <QScrollArea>

PalQt::PalQt(void* pal_parent_ctx, const char* pal_name,
    ldh_tSesContext pal_ldhses, const char* pal_root_name, QWidget** w,
    pwr_tStatus* status)
    : Pal(pal_parent_ctx, pal_name, pal_ldhses, pal_root_name, status)
{
  QScrollArea* scrolledbrow = ((QScrollArea*)scrolledbrowwidgetqt_new(
      Pal::init_brow_cb, this, &brow_widget));

  form_widget = wrapInFrame(scrolledbrow);

  set_inputfocus(0);

  *w = form_widget;
  *status = 1;
}

PalQt::~PalQt()
{
  PalFile::config_tree_free(menu);
  free_pixmaps();
  form_widget->close();
}

void PalQt::create_popup_menu(pwr_tCid cid, int x, int y)
{
  (create_popup_menu_cb)(parent_ctx, cid, x, y);
}

static void modifyBackground(QWidget* widget, QColor color)
{
  QPalette pal;
  pal.setColor(QPalette::Background, color);
  widget->setAutoFillBackground(true);
  widget->setPalette(pal);
}

void PalQt::set_inputfocus(int focus)
{
  if (!displayed) {
    return;
  }

  if (focus) {
    modifyBackground(form_widget, QColor("Black"));
    brow_widget->setFocus();
  } else {
    modifyBackground(form_widget, QColor("White"));
    brow_SetInverseColor(brow_ctx, flow_eDrawType_LineGray);
    selection_owner = 0;
  }
}

void PalQt::set_selection_owner()
{
  selection_owner = 1;
  brow_SetInverseColor(brow_ctx, flow_eDrawType_Line);
  if (!selection_owner) {
    return;
  }

  char name[200];
  brow_tNode* node_list;
  int node_count;
  brow_GetSelectedNodes(brow_ctx, &node_list, &node_count);
  if (!node_count) {
    strcpy(name, "");
  } else {
    PalItem* item;
    brow_GetUserData(node_list[0], (void**)&item);

    switch (item->type) {
    case pal_ePalItemType_ClassVolume:
    case pal_ePalItemType_Class:
    case pal_ePalItemType_Object:
    default:
      brow_GetAnnotation(node_list[0], 0, name, sizeof(name));
      free(node_list);
    }
  }

  QApplication::clipboard()->setText(name, QClipboard::Selection);
}
