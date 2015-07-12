TEMPLATE = subdirs

CONFIG += c++11
CONFIG -= debug_and_release_target


SUBDIRS = ontolis-helpers ontolis-lib ontolis \
    ontolis-avis-node-color ontolis-avis-node-ellipse ontolis-avis-node-expand-status ontolis-avis-node-roundedrect ontolis-avis-node-textarea

ontolis.depends = ontolis-helpers ontolis-lib

ontolis-avis-node-color.depends = ontolis-lib
ontolis-avis-node-ellipse.depends = ontolis-lib
ontolis-avis-node-expand-status.depends = ontolis-lib
ontolis-avis-node-roundedrect.depends = ontolis-lib
ontolis-avis-node-textarea.depends = ontolis-lib
