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

#include "ConfigSubRegionOverlay.hpp"
#include "ClickButton.hpp"
#include "gui.hpp"

ConfigSubRegionOverlay::ConfigSubRegionOverlay(ReplaceableScreen& screen)
    : ReplaceableScreen(&screen, i18n::localize("A_SELECT") + '\n' + i18n::localize("B_BACK")),
      hid(40, 2),
      validSubRegions(i18n::rawSubregions(Configuration::getInstance().language(), Configuration::getInstance().defaultCountry())),
      subregions(validSubRegions)
{
    instructions.addBox(false, 75, 30, 170, 23, COLOR_GREY, i18n::localize("SEARCH"), COLOR_WHITE);
    searchButton = std::make_unique<ClickButton>(75, 30, 170, 23,
        [this]() {
            searchBar();
            return false;
        },
        ui_sheet_emulated_box_search_idx, "", 0, 0);
    hid.update(subregions.size());
    hid.select(std::distance(subregions.begin(), std::find_if(subregions.begin(), subregions.end(), [](const std::pair<u16, std::string>& pair) {
        return pair.first == Configuration::getInstance().defaultRegion();
    })));
}

void ConfigSubRegionOverlay::drawBottom() const
{
    dim();
    Gui::text(i18n::localize("EDITOR_INST"), 160, 115, FONT_SIZE_18, FONT_SIZE_18, COLOR_WHITE, TextPosX::CENTER, TextPosY::TOP);
    searchButton->draw();
    Gui::sprite(ui_sheet_icon_search_idx, 79, 33);
    Gui::text(searchString, 95, 32, FONT_SIZE_12, FONT_SIZE_12, COLOR_WHITE, TextPosX::LEFT, TextPosY::TOP);
}

void ConfigSubRegionOverlay::drawTop() const
{
    Gui::sprite(ui_sheet_part_editor_20x2_idx, 0, 0);
    int x = hid.index() < hid.maxVisibleEntries() / 2 ? 2 : 200;
    int y = (hid.index() % (hid.maxVisibleEntries() / 2)) * 12;
    Gui::drawSolidRect(x, y, 198, 11, COLOR_MASKBLACK);
    Gui::drawSolidRect(x, y, 198, 1, COLOR_YELLOW);
    Gui::drawSolidRect(x, y, 1, 11, COLOR_YELLOW);
    Gui::drawSolidRect(x, y + 10, 198, 1, COLOR_YELLOW);
    Gui::drawSolidRect(x + 197, y, 1, 11, COLOR_YELLOW);
    // Stupid non random-access iterators
    std::map<u8, std::string>::const_iterator locIt = subregions.begin();
    for (size_t i = 0; i < hid.page() * hid.maxVisibleEntries(); i++)
    {
        locIt++;
    }
    for (size_t i = 0; i < hid.maxVisibleEntries(); i++)
    {
        x = i < hid.maxVisibleEntries() / 2 ? 4 : 203;
        if (hid.page() * hid.maxVisibleEntries() + i < subregions.size())
        {
            Gui::text(std::to_string(locIt->first) + " - " + locIt->second, x, (i % (hid.maxVisibleEntries() / 2)) * 12, FONT_SIZE_9, FONT_SIZE_9,
                COLOR_WHITE, TextPosX::LEFT, TextPosY::TOP);
            locIt++;
        }
        else
        {
            break;
        }
    }
}

void ConfigSubRegionOverlay::update(touchPosition* touch)
{
    if (justSwitched && ((hidKeysHeld() | hidKeysDown()) & KEY_TOUCH))
    {
        return;
    }
    else if (justSwitched)
    {
        justSwitched = false;
    }

    if (hidKeysDown() & KEY_X)
    {
        searchBar();
    }
    searchButton->update(touch);

    if (!searchString.empty() && searchString != oldSearchString)
    {
        subregions.clear();
        for (auto i = validSubRegions.begin(); i != validSubRegions.end(); i++)
        {
            std::string locName = i->second.substr(0, searchString.size());
            StringUtils::toLower(locName);
            if (locName == searchString)
            {
                subregions.emplace(*i);
            }
        }
        oldSearchString = searchString;
    }
    else if (searchString.empty() && !oldSearchString.empty())
    {
        subregions      = validSubRegions;
        oldSearchString = searchString = "";
    }
    if (hid.fullIndex() >= subregions.size())
    {
        hid.select(0);
    }

    hid.update(subregions.size());
    u32 downKeys = hidKeysDown();
    if (downKeys & KEY_A)
    {
        if (subregions.size() > 0)
        {
            auto locIt = subregions.begin();
            for (size_t i = 0; i < hid.fullIndex(); i++)
            {
                locIt++;
            }
            Configuration::getInstance().defaultRegion(locIt->first);
        }
        parent->removeOverlay();
        return;
    }
    else if (downKeys & KEY_B)
    {
        parent->removeOverlay();
        return;
    }
}

void ConfigSubRegionOverlay::searchBar()
{
    SwkbdState state;
    swkbdInit(&state, SWKBD_TYPE_NORMAL, 2, 20);
    swkbdSetHintText(&state, i18n::localize("SUBREGION").c_str());
    swkbdSetValidation(&state, SWKBD_ANYTHING, 0, 0);
    char input[25]  = {0};
    SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
    input[24]       = '\0';
    if (ret == SWKBD_BUTTON_CONFIRM)
    {
        searchString = input;
        StringUtils::toLower(searchString);
    }
}
