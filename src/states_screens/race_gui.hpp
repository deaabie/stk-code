//  $Id$
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2004-2005 Steve Baker <sjbaker1@airmail.net>
//  Copyright (C) 2006 Joerg Henrichs, SuperTuxKart-Team, Steve Baker
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_RACEGUI_HPP
#define HEADER_RACEGUI_HPP

#include <string>
#include <vector>

#include "irrlicht.h"
using namespace irr;


#include "config/player.hpp"

class Camera;
class InputMap;
class Kart;
class Material;
class RaceSetup;

class RaceGUI
{
public:
    /**
    * Used to display the list of karts and their times or
    * whatever other info is relevant to the current mode.
    */
    struct KartIconDisplayInfo
    {
        irr::core::stringw time;
        float r, g, b;
        irr::core::stringw special_title;
        /** Current lap of this kart, or -1 if irrelevant
        */
        int lap;
    };   // KartIconDisplayInfo

private:
    class TimedMessage
    {
     public:
        irr::core::stringw m_message;            // message to display
        float              m_remaining_time;     // time remaining before removing this message from screen
        video::SColor      m_color;              // color of message
        int                m_font_size;          // size
        const Kart        *m_kart;
        // -----------------------------------------------------
        // std::vector needs standard copy-ctor and std-assignment op.
        // let compiler create defaults .. they'll do the job, no
        // deep copies here ..
        TimedMessage(const irr::core::stringw &message, 
                     const Kart *kart, float time, int size, 
                     const video::SColor &color)
        {
            m_message        = message; 
            m_font_size      = size;
            m_kart           = kart;
            m_remaining_time = ( time < 0.0f ) ? -1.0f : time;
            m_color          = color;
        }   // TimedMessage
        // -----------------------------------------------------
        // in follow leader the clock counts backwards
        bool done(const float dt)
        {
            m_remaining_time -= dt;
            return m_remaining_time < 0;
        }   // done
    };   // TimedMessage
    // ---------------------------------------------------------

    Material        *m_speed_meter_icon;
    Material        *m_speed_bar_icon;
    Material        *m_plunger_face;
    typedef          std::vector<TimedMessage> AllMessageType;
    AllMessageType   m_messages;
    /** A texture with all mini dots to be displayed in the minimap for all karts. */
    video::ITexture *m_marker;

    /** Musical notes icon (for music description and credits) */
    Material        *m_music_icon;
    
    // Minimap related variables
    // -------------------------
    /** The mini map of the track. */
    video::ITexture *m_mini_map;
    /** The size of a single marker in pixels, must be a power of 2. */
    int              m_marker_rendered_size;

    /** The size of a single marker on the screen for AI karts, 
     *  need not be a power of 2. */
    int              m_marker_ai_size;

    /** The size of a single marker on the screen or player karts, 
     *  need not be a power of 2. */
    int              m_marker_player_size;

    /** The width of the rendered mini map in pixels, must be a power of 2. */
    int              m_map_rendered_width;

    /** The height of the rendered mini map in pixels, must be a power of 2. */
    int              m_map_rendered_height;
    
    /** Width of the map in pixels on the screen, need not be a power of 2. */
    int              m_map_width;

    /** Height of the map in pixels on the screen, need not be a power of 2. */
    int              m_map_height;

    /** Distance of map from left side of screen. */
    int              m_map_left;

    /** Distance of map from bottom of screen. */
    int              m_map_bottom;

    /** The list of all cameras. */
    std::vector<Camera*> m_cameras;

    void createMarkerTexture();
    void createRegularPolygon(unsigned int n, float radius, 
                              const core::vector2df &center,
                              const video::SColor &color,
                              video::S3DVertex *v, unsigned short int *index);

    /* Display informat on screen */
    void drawStatusText        ();
    void drawEnergyMeter       (Kart *player_kart,
                                int   offset_x, int   offset_y,
                                float ratio_x,  float ratio_y  );
    void drawPowerupIcons  (Kart* player_kart,
                                int   offset_x, int   offset_y,
                                float ratio_x,  float ratio_y  );
    void drawAllMessages       (Kart* player_kart,
                                int   offset_x, int   offset_y,
                                float ratio_x,  float ratio_y  );
    void drawPlayerIcons       (const KartIconDisplayInfo* info);
    void oldDrawPlayerIcons    ();
    void drawMiniMap           ();
    void drawTimer             ();
    void drawMusicDescription  ();
    void cleanupMessages       (const float dt);
    void drawSpeed                (Kart* kart, int offset_x, int offset_y,
                                   float ratio_x, float ratio_y           );
    void drawLap                  (const KartIconDisplayInfo* info, Kart* kart, int offset_x,
                                   int offset_y, float ratio_x, float ratio_y  );
public:

         RaceGUI();
        ~RaceGUI();
    void render();
    void update(float dt);
    Camera *addCamera(unsigned int index, Kart *kart);
    void addMessage(const irr::core::stringw &m, const Kart *kart, float time, 
                    int fonst_size, 
                    const video::SColor &color=video::SColor(255, 255, 0, 255));

    /** Returns the size of the texture on which to render the minimap to. */
#if IRRLICHT_VERSION_MAJOR > 1 || IRRLICHT_VERSION_MINOR >= 6
    const core::dimension2du getMiniMapSize() const 
                  { return core::dimension2du(m_map_width, m_map_height); }
#else
    const core::dimension2di getMiniMapSize() const 
                  { return core::dimension2di(m_map_width, m_map_height); }
#endif
};

#endif
