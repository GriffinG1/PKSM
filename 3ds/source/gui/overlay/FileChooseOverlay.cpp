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

#include "FileChooseOverlay.hpp"
#include "gui.hpp"
#include "i18n.hpp"

FileChooseOverlay::FileChooseOverlay(Screen& screen, std::vector<std::string>& strings, const std::string& rootString)
    : Overlay(screen, i18n::localize("A_SELECT") + '\n' + i18n::localize("B_BACK")), currDirString("/"), rootString(rootString), currDir("/"), strings(strings), hid(9, 1)
{
    updateEntries();
}

void FileChooseOverlay::drawTop() const
{
    // slight change to stripy top
    for (int x = 0; x < 400; x += 7)
    {
        for (int y = 0; y < 240; y += 7)
        {
            Gui::sprite(ui_sheet_bg_stripe_top_idx, x, y);
        }
    }
    Gui::drawSolidRect(0, 0, 400, 20, C2D_Color32(15, 22, 89, 255));

    // Leaving space for the icon
    Gui::text(currDirString, 15, 2, FONT_SIZE_11, FONT_SIZE_11, COLOR_YELLOW, TextPosX::LEFT, TextPosY::TOP);

    Gui::drawSolidRect(0, 20 + hid.index() * 25, 400, 25, C2D_Color32(128, 128, 128, 255));
    Gui::drawSolidRect(1, 21 + hid.index() * 25, 398, 23, COLOR_MASKBLACK);

    for (size_t i = hid.page() * hid.maxVisibleEntries(); i < (hid.page() + 1) * hid.maxVisibleEntries(); i++)
    {
        if (i >= currFiles.size())
        {
            break;
        }
        else
        {
            Gui::sprite(currFiles[i].second ? ui_sheet_icon_folder_idx : ui_sheet_icon_script_idx, 3, 23 + i % hid.maxVisibleEntries() * 25);
            Gui::text(currFiles[i].first, 30, 24 + (i % hid.maxVisibleEntries() * 25), FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::LEFT,
                TextPosY::TOP);
        }
    }
}

void FileChooseOverlay::updateEntries()
{
    hid.select(0);
    currFiles.clear();
    if (!currDir.good())
    {
        currFiles.push_back({i18n::localize("FOLDER_DOESNT_EXIST"), false});
        return;
    }
    for (size_t i = 0; i < currDir.count(); i++)
    {
        std::string item = currDir.item(i);
        if (item != "." && item != "..")
        {
            currFiles.push_back(std::make_pair(item, currDir.folder(i)));
        }
    }
    std::sort(currFiles.begin(), currFiles.end(), [this](std::pair<std::string, bool>& first, std::pair<std::string, bool>& second) {
        if ((first.second && second.second) || (!first.second && !second.second))
        {
            return first.first < second.first;
        }
        else if (first.second)
        {
            return true;
        }
        else
        {
            return false;
        }
    });
}

void FileChooseOverlay::update(touchPosition* touch)
{
    hid.update(currFiles.size());
    u32 down = hidKeysDown();
    if (down & KEY_B)
    {
        if (currDirString == "/")
        {
            screen.removeOverlay();
            return;
        }
        else
        {
            currDirString = currDirString.substr(0, currDirString.substr(0, currDirString.size() - 1).find_last_of('/') + 1);
            currDir       = STDirectory(currDirString);
            updateEntries();
        }
    }
    else if (down & KEY_A)
    {
        if (currFiles[hid.fullIndex()].second)
        {
            currDirString += currFiles[hid.fullIndex()].first + '/';
            currDir = STDirectory(currDirString);
            updateEntries();
        }
        else
        {
            if (Gui::showChoiceMessage(i18n::localize("FILE_CONFIRM_CHOICE"), '\'' + currFiles[hid.fullIndex()].first + '\''))
            {
                strings.emplace_back(currDirString + currFiles[hid.fullIndex()].first);
                screen.removeOverlay();
            }
        }
    }
}