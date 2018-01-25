import io
import os

# Imports the Google Cloud client library
from google.cloud import vision
from google.cloud.vision import types
import sys, getopt


def main(argv):
    
    file_name = sys.argv[1]

    client = vision.ImageAnnotatorClient()

    # Loads the image into memory
    with io.open(file_name, 'rb') as image_file:
        content = image_file.read()

    image = types.Image(content=content)

    # Performs label detection on the image file
    response = client.label_detection(image=image)
    labels = response.label_annotations
    fileWrite = open("googleResult.txt",'w')
    print('Labels:')
    for label in labels:
        fileWrite.write(label.description + "\t" + str(label.score) + "\n")
    fileWrite.close()

if __name__ == "__main__":
    main(sys.argv[1:])