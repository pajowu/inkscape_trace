// SPDX-License-Identifier: GPL-2.0-or-later
/**
 * @file
 * Inktrace - An SVG file viewer.
 */
/*
 * Authors:
 *   Tavmjong Bah
 *   Karl Engelhardt
 *
 * Copyright (C) 2020 Authors
 *
 * The contents of this file may be used under the GNU General Public License Version 2 or later.
 * Read the file 'COPYING' for more information.
 *
 */

#include <gtkmm.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/notebook.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/stack.h>


#include <glibmm/i18n.h>

#include "desktop.h"
#include "selection.h"

#include "inkscape.h"
#include "io/resource.h"
#include "io/sys.h"
#include "trace/autotrace/inkscape-autotrace.h"
#include "trace/potrace/inkscape-potrace.h"
#include "trace/depixelize/inkscape-depixelize.h"

#include <iostream>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    Inkscape::GC::init();
    Inkscape::Application::create(false);

    Inkscape::Trace::Potrace::TraceType potraceType = Inkscape::Trace::Potrace::TRACE_QUANT_COLOR;

    // ********* START PARAMETERS
    Inkscape::Trace::Potrace::PotraceTracingEngine pte(
        potraceType, // TraceType traceType,
        false, // bool invert
        4, // int quantizationNrColors (for single run)
        0., // double brightnessThreshold,
        0., // double brightnessFloor,
        0., // double cannyHighThreshold,
        4, // int multiScanNrColors,
        true, // bool multiScanStack,
        false, // bool multiScanSmooth,
        true // bool multiScanRemoveBackground
    );

    // *** SET THOSE IF YOU WANT
    // pte.potraceParams->opticurve = CB_optimize->get_active();
    // pte.potraceParams->opttolerance = optimize->get_value();
    // pte.potraceParams->alphamax = CB_smooth->get_active() ? smooth->get_value() : 0;
    // pte.potraceParams->turdsize = CB_speckles->get_active() ? (int)speckles->get_value() : 0;6

    // ********* END PARAMETERS

    for (int i = 1; i < argc; ++i)
    {
        Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file(argv[i]);
        auto svgname = fs::path(argv[i]);
        svgname.replace_extension(".svg");

        auto trace_results = pte.trace(pixbuf);


        std::ofstream myfile;
        std::cout << "writing to " << svgname << "\n";
        myfile.open(svgname);
        myfile << "<svg width=\"250\" height=\"250\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">";
        // ********** HACK: the layers in your output svgs are in the wrong order, switch which for loop is active
        // for (auto trace_result: trace_results) {
        for (unsigned j = trace_results.size(); j-- > 0; ) {
            auto trace_result = trace_results[j];
            myfile << "<path style=\"" << trace_result.getStyle() << "\" d=\"" << trace_result.getPathData() << "\" />";
        }
        myfile << "</svg>";
    }
    return 0;
}
