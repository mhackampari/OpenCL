import os
import numpy
import csv

print("Loading ", os.path.basename(__file__))

# these variables are public
__all__ = ["var1", "var2"]

# protected or private
_x = 0
"""
Docstring
"""


class Data:
    """Initialize data

    Initialize all data with user/default value parameter
    """


    def __init__(self, filename="data", numelem=100, maxweight=20, verbose=False):
        """Form a complex number.

        Keyword arguments:
        numelem -- number of elements to be initialized (default=100)
        maxweight -- the upper limit for random generation: weight in [1..20] (default=20)
        verbose -- verobsity flag for stdout
        """

        self.__numelem = numelem
        self.weights = numpy.random.random_integers(1, maxweight, size=self.__numelem).astype(numpy.uint32)
        self.values = numpy.random.random_integers(1, maxweight*2, size=self.__numelem).astype(numpy.uint32)
        self.sumofweights = numpy.uint32(self.weights.sum())
        self.capacity = numpy.uint32(self.sumofweights/2)
        self.f0 = numpy.zeros(self.capacity+1).astype(numpy.uint32)
        self.f1 = numpy.zeros_like(self.f0)
        self.m_d = numpy.zeros_like(self.f0)

        """Export generated data to file"""

        self.filename = filename + str(numelem)
        weightsandvalues = list(zip(self.weights, self.values))

        with open(self.filename+".csv", "w", newline='') as csvfile:

            fieldnames = ["Capacity", "Weight", "Value", "KnapsackW", "KnapsackV", "AvgTime"]
            csvwriter = csv.DictWriter(csvfile, fieldnames=fieldnames)
            csvwriter.writeheader()

            tuple = weightsandvalues[0]
            csvwriter.writerow({'Weight': tuple[0], 'Value': tuple[1], 'Capacity': self.capacity})

            for tuple in weightsandvalues[1:]:
                csvwriter.writerow({'Weight': tuple[0], 'Value': tuple[1]})

            csvfile.close()

    def __str__(self):

        prtsumw = "TOTAL WEIGHTS: {}\n".format(self.sumofweights)
        prtcapacity = "CAPACITY: {}\n".format(self.capacity)
        prtnumoflelems = "NUMBER OF ELEMENTS: {}\n".format(self.__numelem)
        prtweights = "WEIGHTS: {}\n".format(self.weights)
        prtvalues = "VALUES: {}\n".format(self.values)
        return "{} {} {} {} {}".format(prtsumw, prtcapacity, prtnumoflelems, prtweights, prtvalues)


print("Ended loading ", os.path.basename(__file__))

# Used when it runs as standalone file (when it is not imported)
# Used for unit tests
if __name__ == "__main__":
    numelem = 10
    filename = "data"
    mytest = Data(numelem=numelem)
    try:

        with open(filename+str(numelem)+".csv", "r", newline="") as csvfile:
            read_header = ['KnapsackW', 'KnapsackV', 'AvgTime']
            csvreader = csv.DictReader(csvfile)

            print("reading file csv")
            # reading the first line
            line = next(csvreader)
            capacity = int(line["Capacity"])
            sumweights = int(line["Weight"])
            print(line)

            for i, line in enumerate(csvreader):
                # i+1 because the first line has been read above
                print(i+1, line["Weight"], line["Value"])
                sumweights += int(line["Weight"])
            csvfile.close()
            print(sumweights//2)
            assert(sumweights//2 == capacity)

    except AssertionError as e:
        print(e)
    else:

        print("Test passed")