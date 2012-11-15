/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   Product name: redemption, a FLOSS RDP proxy
 *   Copyright (C) Wallix 2010-2012
 *   Author(s): Christophe Grosjean, Dominique Lafages, Jonathan Poelen
 */

#if !defined(__MAIN_RECORDER_RECORDER_APP__)
#define __MAIN_RECORDER_RECORDER_APP__

#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <utility>
#include <string>

#include "capture.hpp"
#include "FileToGraphic.hpp"

int recorder_app(int argc, char** argv)
{
    openlog("redrec", LOG_CONS | LOG_PERROR, LOG_USER);

    std::string input_filename;
    std::string output_filename;
    uint32_t begin_cap = 0;
    uint32_t end_cap = 0;
    uint32_t png_limit = 10;
    uint32_t png_interval = 60;
    uint32_t frame_interval = 100;
    uint32_t break_interval = 86400;

    boost::program_options::options_description desc("Options");
    desc.add_options()
    ("help,h", "produce help message")
    ("version,v", "show software version")
    ("output-file,o", boost::program_options::value(&output_filename), "output base filename (see --output-type)")
    ("input-file,i", boost::program_options::value(&input_filename), "input base filename (see --input-type)")
    ("begin,b", boost::program_options::value<uint32_t>(&begin_cap), "begin capture time (in seconds), default=none")
    ("end,e", boost::program_options::value<uint32_t>(&end_cap), "end capture time (in seconds), default=none")
    ("pnglimit,l", boost::program_options::value<uint32_t>(&png_limit), "maximum number of png files to create (remove older), default=10, 0 will disable png capture")
    ("pnginterval,n", boost::program_options::value<uint32_t>(&png_interval), "time interval between png captures, default=60 seconds")
    ("frameinterval,f", boost::program_options::value<uint32_t>(&frame_interval), "time between consecutive capture frames (in 100/th of seconds), default=100 one frame per second")
    ("breakinterval,f", boost::program_options::value<uint32_t>(&break_interval), "number of seconds between splitting wrm files in seconds(default, one wrm every day)")
    ("wrm,w", "enable wrm capture")
    ;

    boost::program_options::variables_map options;
    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv).options(desc)
//            .positional(p)
            .run(),
        options
    );
    boost::program_options::notify(options);

    if (input_filename.c_str()[0] == 0){
        cout << desc << endl;
        printf("Missing input filename\n");
        exit(-1);
    }

    if (output_filename.c_str()[0] == 0){
        printf("Missing output filename\n");
        cout << desc << endl;
        exit(-1);
    }

    Inifile ini;
    ini.globals.png_limit = png_limit;
    ini.globals.png_interval = png_interval;
    ini.globals.frame_interval = frame_interval;
    ini.globals.break_interval = break_interval;

    timeval begin_capture;
    begin_capture.tv_sec = begin_cap; begin_capture.tv_usec = 0;
    timeval end_capture;
    end_capture.tv_sec = end_cap; end_capture.tv_usec = 0;

    InByMetaSequenceTransport in_wrm_trans(input_filename.c_str());
    FileToGraphic player(&in_wrm_trans, begin_capture, end_capture, false);

    TODO("we should manage direct choice of the right start chunk based on content of mwrm, passing start capture to mwrm start chunk should be enough.")
    TODO("Also it should reject chunk change after end_capture point, but this is less critical as we must manage detecting stop from inside chunk anyway")
    // less than 1 year means we a re given a time relatve to beginning of movie
    if (begin_cap && (begin_cap < 31536000)){ // less than 1 year, it is relative not absolute timestamp
        printf("relative capture\n");
        // begin_capture.tv_usec is 0
        player.begin_capture.tv_usec = player.record_now.tv_usec;
        player.begin_capture.tv_sec = player.record_now.tv_sec + begin_cap;
    }

    if (end_cap && (end_cap < 31536000)){
        printf("relative capture\n");
         // begin_capture.tv_usec is 0
        player.end_capture.tv_usec = player.record_now.tv_usec;
        player.end_capture.tv_sec = player.record_now.tv_sec + end_cap;
    }

    TODO("Factorize this, see similar code in front, start_capture")
    const char * fullpath = output_filename.c_str();
    char path[1024];
    char basename[1024];
    strcpy(path, "./"); // default value, actual one should come from movie_path
    strcpy(basename, "redemption"); // default value actual one should come from movie_path

    canonical_path(fullpath, path, sizeof(path), basename, sizeof(basename));

    Capture capture(player.record_now, player.screen_rect.cx, player.screen_rect.cy, path, basename, options.count("wrm") > 0, ini);

TODO("Capture is not a drawable, this is a problem when replaying as it won't get the image chunks. We should change API to make both RDPGraphicDevice and RDPDrawable able to receive image chunks (structures that are not drawables won't do anything with it, that's all. Hence we can't set player using add_consumer(capture) or we won't get image chunks")

    TODO("making change above woud be nice because the current state relies on many objects instanciated inside capture")
    if (options.count("wrm") > 0){
        player.add_consumer(capture.drawable);
        player.add_consumer(capture.pnc);
    }
    if (ini.globals.png_limit > 0){
        player.add_consumer(capture.psc);
    }

    player.play();

    return 0;
}


#endif
