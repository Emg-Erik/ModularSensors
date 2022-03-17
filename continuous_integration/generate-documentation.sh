#!/bin/sh

# Script modified from scripts by Jeroen de Bruijn, thephez, and Adafruit
# https://gist.github.com/vidavidorra/548ffbcdae99d752da02
# https://github.com/thephez/doxygen-travis-build
# https://learn.adafruit.com/the-well-automated-arduino-library/travis-ci

# Exit with nonzero exit code if anything fails
set -e

cd $TRAVIS_BUILD_DIR/code_docs/m.css
echo "\n\e[32mUpdate the style sheets\e[0m"
cd $TRAVIS_BUILD_DIR/code_docs/m.css/css/EnviroDIY
python $TRAVIS_BUILD_DIR/code_docs/m.css/css/postprocess.py "m-EnviroDIY.css"
python $TRAVIS_BUILD_DIR/code_docs/m.css/css/postprocess.py "m-EnviroDIY.css" "m-documentation.css" -o "m-EnviroDIY+documentation.compiled.css"
python $TRAVIS_BUILD_DIR/code_docs/m.css/css/postprocess.py "m-EnviroDIY.css" "m-theme-EnviroDIY.css" "m-documentation.css" --no-import -o "m-EnviroDIY.documentation.compiled.css"
cp $TRAVIS_BUILD_DIR/code_docs/m.css/css/EnviroDIY/m-EnviroDIY+documentation.compiled.css $TRAVIS_BUILD_DIR/code_docs/ModularSensors/docs/css

cd $TRAVIS_BUILD_DIR/code_docs/ModularSensors/docs

# echo "\n\e[32mCreating dox files from example read-me files\e[0m"
# python documentExamples.py

echo "\n\e[32mCurrent Doxygen version...\e[0m"
$TRAVIS_BUILD_DIR/doxygen-src/build/bin/doxygen -v 2>&1

# Redirect both stderr and stdout to the log file AND the console.
echo "\n\e[32mGenerating Doxygen code documentation...\e[0m"
$TRAVIS_BUILD_DIR/doxygen-src/build/bin/doxygen Doxyfile 2>&1 | tee doxygen.log

echo "\n\e[32mFixing errant xml section names in examples as generated by Doxygen...\e[0m"
python fixSectionsInXml.py

# echo "\n\e[32mFixing copied function documentation in group documentation\e[0m"
# python fixFunctionsInGroups.py

python $TRAVIS_BUILD_DIR/code_docs/m.css/documentation/doxygen.py "mcss-conf.py" --no-doxygen --output output_mcss.log --templates "$TRAVIS_BUILD_DIR/code_docs/m.css/documentation/templates/EnviroDIY" --debug

echo "\n\e[32mCopying function documentation\e[0m"
python copyFunctions.py