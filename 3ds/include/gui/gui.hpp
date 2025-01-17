/*
 *   This file is part of PKSM
 *   Copyright (C) 2016-2019 Bernardo Giordano, Admiral Fish, piepie62
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
 *       * Requiring preservation of specified reasonable legal notices or
 *         author attributions in that material or in the Appropriate Legal
 *         Notices displayed by works containing it.
 *       * Prohibiting misrepresentation of the origin of that material,
 *         or requiring that modified versions of such material be marked in
 *         reasonable ways as different from the original version.
 */

#ifndef GUI_HPP
#define GUI_HPP

#include "PKX.hpp"
#include "Sav.hpp"
#include "Screen.hpp"
#include "TextParse.hpp"
#include "TextPos.hpp"
#include "WCX.hpp"
#include "colors.hpp"
#include "i18n.hpp"
#include "json.hpp"
#include "mixer.hpp"
#include "pkm_spritesheet.h"
#include "thread.hpp"
#include "types_spritesheet.h"
#include "ui_sheet.h"
#include "utils.hpp"
#include <3ds.h>
#include <citro2d.h>
#include <random>
#include <stack>
#include <string.h>
#include <unordered_map>

// emulated
#define ui_sheet_res_null_idx 500
#define ui_sheet_emulated_pointer_horizontal_flipped_idx 501
#define ui_sheet_emulated_bg_top_red 502
#define ui_sheet_emulated_bg_top_blue 503
#define ui_sheet_emulated_bg_top_green 504
#define ui_sheet_emulated_bg_bottom_red 505
#define ui_sheet_emulated_bg_bottom_blue 506
#define ui_sheet_emulated_bg_bottom_green 507
#define ui_sheet_emulated_eventmenu_bar_selected_flipped_horizontal_idx 508
#define ui_sheet_emulated_eventmenu_bar_unselected_flipped_horizontal_idx 509
#define ui_sheet_emulated_eventmenu_bar_selected_flipped_vertical_idx 510
#define ui_sheet_emulated_eventmenu_bar_unselected_flipped_vertical_idx 511
#define ui_sheet_emulated_eventmenu_bar_selected_flipped_both_idx 512
#define ui_sheet_emulated_eventmenu_bar_unselected_flipped_both_idx 513
#define ui_sheet_emulated_storage_box_corner_flipped_horizontal_idx 514
#define ui_sheet_emulated_storage_box_corner_flipped_vertical_idx 515
#define ui_sheet_emulated_storage_box_corner_flipped_both_idx 516
#define ui_sheet_emulated_toggle_green_idx 517
#define ui_sheet_emulated_toggle_red_idx 518
#define ui_sheet_emulated_gameselector_bg_idx 519
#define ui_sheet_emulated_button_qr_idx 520
#define ui_sheet_emulated_button_item_idx 521
#define ui_sheet_emulated_button_plus_small_black_idx 522
#define ui_sheet_emulated_button_minus_small_black_idx 523
#define ui_sheet_emulated_box_search_idx 524
#define ui_sheet_emulated_toggle_gray_idx 525
#define ui_sheet_emulated_toggle_blue_idx 526
#define ui_sheet_emulated_party_indicator_1_idx 527
#define ui_sheet_emulated_party_indicator_2_idx 528
#define ui_sheet_emulated_party_indicator_3_idx 529
#define ui_sheet_emulated_party_indicator_4_idx 530
#define ui_sheet_emulated_party_indicator_5_idx 531
#define ui_sheet_emulated_party_indicator_6_idx 532
#define ui_sheet_emulated_button_selected_blue_idx 533
#define ui_sheet_emulated_button_unselected_blue_idx 534
#define ui_sheet_emulated_button_unavailable_blue_idx 535
#define ui_sheet_emulated_button_selected_red_idx 536
#define ui_sheet_emulated_button_unselected_red_idx 537
#define ui_sheet_emulated_button_unavailable_red_idx 538
#define ui_sheet_emulated_button_pouch_idx 539
#define ui_sheet_emulated_textbox_illegal_idx 540
#define ui_sheet_emulated_bg_top_yellow_idx 541
#define ui_sheet_emulated_bg_bottom_yellow_idx 542
#define ui_sheet_emulated_button_lang_enabled_idx 543
#define ui_sheet_emulated_button_lang_disabled_idx 544
#define ui_sheet_emulated_stripe_move_grey_idx 545
// These are both about 18x18
#define ui_sheet_emulated_button_filter_positive_idx 546
#define ui_sheet_emulated_button_filter_negative_idx 547
#define ui_sheet_emulated_button_tab_unselected_idx 548

#define FONT_SIZE_18 0.72f
#define FONT_SIZE_15 0.6f
#define FONT_SIZE_14 0.56f
#define FONT_SIZE_12 0.50f
#define FONT_SIZE_11 0.46f
#define FONT_SIZE_9 0.37f

namespace Gui
{
    Result init(void);
    void mainLoop(void);
    void exit(void);

    void target(gfxScreen_t t);
    void clearScreen(gfxScreen_t t);
    void flushText();
    C2D_Image TWLIcon(void);

    void ball(size_t index, int x, int y);
    void type(Language lang, u8 type, int x, int y);
    void generation(const PKX& pkm, int x, int y);
    void format(const PKX& pkm, int x, int y);
    void sprite(int key, int x, int y);
    void sprite(int key, int x, int y, u32 color);
    void pkm(const PKX& pkm, int x, int y, float scale = 1.0f, u32 color = C2D_Color32(0, 0, 0, 255), float blend = 0.0f);
    void pkm(int species, int form, Generation generation, int gender, int x, int y, float scale = 1.0f, u32 color = C2D_Color32(0, 0, 0, 255),
        float blend = 0.0f);

    int pointerBob();

    void drawImageAt(const C2D_Image& img, float x, float y, const C2D_ImageTint* tint = nullptr, float scaleX = 1.0f, float scaleY = 1.0f);
    void drawSolidRect(float x, float y, float w, float h, u32 color);
    void drawSolidCircle(float x, float y, float radius, u32 color);
    void drawSolidTriangle(float x1, float y1, float x2, float y2, float x3, float y3, u32 color);
    void drawLine(float x1, float y1, float x2, float y2, float thickness, u32 color);

    void backgroundTop(bool stripes);
    void backgroundBottom(bool stripes);
    void backgroundAnimatedTop(void);
    void backgroundAnimatedBottom(void);
    void setDoHomeDraw(void);
    void drawNoHome(void);
    void drawSelector(float x, float y);

    std::shared_ptr<TextParse::Text> parseText(const std::string& str, float scaleX, float maxWidth = 0.0f);
    void clearText(void);
    void text(
        const std::shared_ptr<TextParse::Text> text, float x, float y, float scaleX, float scaleY, u32 color, TextPosX positionX, TextPosY positionY);
    void text(const std::string& str, float x, float y, float scaleX, float scaleY, u32 color, TextPosX positionX, TextPosY positionY,
        float maxWidth = 0.0f);
    // Only works with single lines, because why the fuck would you wrap AND scroll?
    void scrollingText(
        const std::string& str, float x, float y, float scaleX, float scaleY, u32 color, TextPosX positionX, TextPosY positionY, int width);
    void slicedText(
        const std::string& str, float x, float y, float scaleX, float scaleY, u32 color, TextPosX positionX, TextPosY positionY, int width);

    void setScreen(std::unique_ptr<Screen> screen);
    void screenBack(void);
    bool showChoiceMessage(const std::string& message, std::optional<std::string> message2 = std::nullopt, int timer = 0);
    void showRestoreProgress(u32 partial, u32 total);
    void showDownloadProgress(const std::string& path, u32 partial, u32 total);
    void waitFrame(const std::string& message, std::optional<std::string> message2 = std::nullopt);
    void warn(const std::string& message, std::optional<std::string> message2 = std::nullopt, std::optional<Language> forceLang = std::nullopt);
    void error(const std::string& message, Result errorCode);
    void showResizeStorage(void);
}

#endif
