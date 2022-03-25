#!/usr/bin/python

ofile = open('ThroughPlaneSVG.h', 'w')

ofile.write("""/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THROUGPLANESVG_H
#define THROUGPLANESVG_H

const char * ThroughPlaneSVGString = \"""")

with open("../../../share/ThermalThroughPlaneSetup.svg", "r") as f:
	for line in f:
		line = line.replace('  ', ' ')
		line = line.replace('  ', ' ')
		line = line.replace('  ', ' ')
		line = line.replace('  ', ' ')
		line = line.replace('  ', ' ')
		
		line = line.replace('"', '\\"')
		line = line.replace('\n', '')
		ofile.write (line)

ofile.write ("""\";
#endif
""")

ofile.close();

ofile = open('TwoSThermalTestStructureSVG.h', 'w')

ofile.write("""/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef TWOSTHERMALTESTSTRUCTURE_H
#define TWOSTHERMALTESTSTRUCTURE_H

const char * TwoSThermalTestStructureSVGString = \"""")

with open("../../../share/2SThermalTestStructure.svg", "r") as f:
	for line in f:
		line = line.replace('  ', ' ')
		line = line.replace('  ', ' ')
		line = line.replace('  ', ' ')
		line = line.replace('  ', ' ')
		line = line.replace('  ', ' ')
		
		line = line.replace('"', '\\"')
		line = line.replace('\n', '')
		ofile.write (line)
		
ofile.write ("""\";
#endif
""")

ofile.close();
