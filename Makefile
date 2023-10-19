all: driver

driver: driver.cpp
	g++ driver.cpp -o driver

clean:
	rm -f predictors