#!/usr/bin/python3

from sys import argv, stdout, exit
from os.path import dirname
from xml.etree.ElementTree import parse
from skimage.io import imread, imsave, use_plugin

use_plugin('freeimage')

if len(argv) != 3:
    stdout.write('Usage: %s labelling_path output_dir\n' % argv[0])
    exit(1)

labelling_path = argv[1]
dir_path = dirname(labelling_path)
output_dir = argv[2] + '/'

batch_tag = parse(labelling_path).getroot()

physical_signs = {}
for photo_tag in batch_tag.findall('Photo'):
    bboxes = photo_tag.find('BBoxes')
    if bboxes is None:
        bboxes = []
    else:
        bboxes = bboxes.findall('BBox')

    for bbox_tag in bboxes:
        bbox_id = int(bbox_tag.attrib['ID'])
        x = int(bbox_tag.find('X').text)
        y = int(bbox_tag.find('Y').text)
        w = int(bbox_tag.find('W').text)
        h = int(bbox_tag.find('H').text)

        if w < 16 or h < 16:
          continue

        if bbox_id not in physical_signs:
            physical_signs[bbox_id] = []

        value = {'photo': photo_tag.find('File').text,
                 'x': x, 'y': y, 'w': w, 'h': h}
        physical_signs[bbox_id].append(value)

k = 0
fhandle = open(output_dir + '/in_labelling.txt', 'w')
for (bbox_id, values) in physical_signs.items():
    fhandle.write('%d %d' % (bbox_id, len(values)))
    for v in values:
        img = imread(dir_path + '/' + v['photo'])
        sign = img[v['y']:v['y'] + v['h'], v['x']:v['x'] + v['w'], :]
        filename = ('%05d.jpg' % k)
        k += 1
        imsave(output_dir + filename, sign)
        fhandle.write(' %s' % filename)
    fhandle.write('\n')

