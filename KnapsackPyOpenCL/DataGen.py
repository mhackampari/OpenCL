import os
import numpy
import math

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


    def __init__(self, numelem=100, maxweight=20, verbose=False):
        """Form a complex number.

        Keyword arguments:
        numelem -- number of elements to be initialized (default=100)
        maxweight -- the upper limit for random generation: weight in [1..20] (default=20)
        verbose -- verobsity flag for stdrout
        """

        self.__numelem = numelem
        self.weights = numpy.random.random_integers(1, maxweight, size=self.__numelem).astype(numpy.uint32)
        self.values = numpy.random.random_integers(1, maxweight*2, size=self.__numelem).astype(numpy.uint32)
        self.sumofweights = self.weights.sum()
        self.capacity = numpy.uint32(self.sumofweights/2)
        self.f0 = numpy.zeros(self.capacity+1).astype(numpy.uint32)
        self.f1 = numpy.zeros_like(self.f0)
        self.m_d = numpy.zeros_like(self.f0)

        """Expport generated data to file"""

        filename = "data" + str(numelem)
        weightsandvalues = list(zip(self.weights, self.values))
        with open(filename, "w") as file:
            file.write("Capacity: %d\n" % self.capacity)
            for tuple in weightsandvalues:
                file.write("w: {0}  v: {1}\n".format(tuple[0], tuple[1]))
            file.close

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
    mytest = Data(numelem=10)
    try:
        with open("data"+str(10), "r") as file:
            #print(file.readline()[-2])
            #print(file.readlines())

            try:
                tmpline = file.readline()
                tmpCap = tmpline.split()[-1]
                assert(numpy.uint32(tmpCap) == mytest.capacity)
                print("Capacity OK: ", mytest.capacity)

            except AssertionError as inst:
                print(inst)
                print("Capacity are not equal, or wrong formatting")
                print("Control: {} vs {} values".format(tmpline, mytest.capacity))
                file.close()

            weightiter = iter(mytest.weights)
            for index, line in enumerate(file.readlines()):
                tmpWeight = line.split()[1]
                try:
                    assert(numpy.uint32(tmpWeight) == next(weightiter))
                except AssertionError as inst:
                    print(inst)
                    print(("File's object is different from generated object at line: ", index))
                    print("Mismatch happened on: ", line[1])
                    file.close()

    except Exception as e:
        print(e)
    else:
        print("Test passed")