from matplotlib import pyplot
import csv
import io
import glob

for filename in glob.glob("./*.galog"):
	file = open(filename, newline='')

	x_args = []
	y_args_min = []
	y_args_max = []
	y_args_avg = []

	for line in csv.reader(file, delimiter=" ", skipinitialspace=True):
		x_args.append(float(line[0]))
		y_args_avg.append(float(line[4]))
		y_args_min.append(float(line[5]))
		y_args_max.append(float(line[6]))

	#pyplot.xticks(x_args[0::50])
	#pyplot.xticks(y_args[0::50])

	pyplot.plot(x_args, y_args_min, color='blue')
	pyplot.plot(x_args, y_args_avg, color='red')
	pyplot.plot(x_args, y_args_max, color='green')
	pyplot.title(filename, fontsize='10')
	pyplot.tight_layout()
	pyplot.savefig(filename.replace("galog", "png"))
	pyplot.cla()
	#pyplot.show()