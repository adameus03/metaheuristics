from matplotlib import pyplot
import csv
import io
import glob

for filename in glob.glob("./*.salog"):
	file = open(filename, newline='')

	x_args = []
	y_args = []

	for line in csv.reader(file, delimiter=" "):
		x_args.append(float(line[0]))
		y_args.append(float(line[16]))

	#pyplot.xticks(x_args[0::50])
	#pyplot.xticks(y_args[0::50])

	pyplot.plot(x_args, y_args)
	pyplot.tight_layout()
	pyplot.savefig(filename.replace("salog", "png"))
	pyplot.cla()
	#pyplot.show()