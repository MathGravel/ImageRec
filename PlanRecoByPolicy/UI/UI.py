from cStringIO import StringIO
from Tkinter import *
from PIL import ImageTk
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import pydot
import time



class UI:
	def __init__(self, master):

		# convert from networkx -> pydot
		pydot_graph = pydot.graph_from_dot_file('Simple.dot')[0]
		# render pydot by calling dot, no file saved to disk
		image = pydot_graph.create_png()
		phImage = ImageTk.PhotoImage(image)

		frame = Frame(master)
		frame = Label(root, image=phImage)
		frame.pack()




# plot the image
root  = Tk()

test = UI(root)

root.mainloop()

# convert from networkx -> pydot
pydot_graph = pydot.graph_from_dot_file('Simple.dot')[0]
# render pydot by calling dot, no file saved to disk
png_str = pydot_graph.create_png(prog='dot')

# treat the dot output string as an image file
sio = StringIO()
sio.write(png_str)
sio.seek(0)
img = mpimg.imread(sio)

# plot the image
imgplot = plt.imshow(img, aspect='equal')
plt.show(block=False)
time.sleep(5)

