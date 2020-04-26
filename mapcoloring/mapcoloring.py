#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Apr 18 20:42:09 2020

@author: itamar
"""

import numpy as np
import pandas as pd
import warnings
warnings.filterwarnings("ignore")

class colored_map():
    
    colors = ['blue','black','red','green']
    
    def  __init__(self,neighbors,countries):
        self.neighbors = neighbors
        self.states = countries
        self.number_of_countries = np.array(len(countries))
        self.number_of_neighbors = dict(zip(neighbors.keys(),[len(x) for x in neighbors.values()]))
        self.country_color = {}
    def color_map(self):
        to_be_colored = self.number_of_neighbors
        color_index = np.random.randint(0,4)
        for i in range(self.number_of_countries):
            country = max()

countries = ['BR','PR','UR','AR','CH','BO','PE','EC','CO','VN','GY','SU','GF']

neighbors = {}
neighbors[countries[0]] = ['PR','UR','AR','BO','PE','CO','VN','GY','SU','GF']
neighbors[countries[1]] = ['BR','AR','BO']
neighbors[countries[2]] = ['BR','AR']
neighbors[countries[3]] = ['BR','UR','PR','CH','BO']
neighbors[countries[4]] = ['AR','BO','PE']
neighbors[countries[5]] = ['PR','AR','CH','BR','PE']
neighbors[countries[6]] = ['BO','CH','EC','BR','CO']
neighbors[countries[7]] = ['PR','CO']
neighbors[countries[8]] = ['EC','PE','BR','VN']
neighbors[countries[9]] = ['CO','BR','GY']
neighbors[countries[10]] = ['VN','BR','SU']
neighbors[countries[11]] = ['GY','BR','GF']
neighbors[countries[12]] = ['SU','BR']

# checking if its correct 
#print(len(neighbors['BR']) + 2 == len(countries) -1) #subtrai o próprio
        
colors = ['blue','black','red','green']      
neighbors = neighbors
states = countries
number_of_countries = np.array(len(countries))
number_of_neighbors = dict(zip(neighbors.keys(),[len(x) for x in neighbors.values()]))
country_color = {}

temporary_neighbors = pd.DataFrame(number_of_neighbors.values())

def get_all_neighbors_from_color(neighbors,this_color_countries):
    all_neighbors_list = []
    
    for cont in this_color_countries:
        for neigb in neighbors[cont]:
            all_neighbors_list.append(neigb)
                                   
    return all_neighbors_list

#get_all_neighbors_from_color(neighbors,this_color_countries)



already_colored = []
first_color = 0
for color in colors:
    this_color_countries = []
    temporary_neighbors = pd.DataFrame(number_of_neighbors.values())
    for i in range(len(countries)):
        country = temporary_neighbors.idxmax().values[0]
        max_value = temporary_neighbors.ix[country];
        
        if first_color == 0:
            country_color[countries[country]] = color
            first_color = 1
            #old_max_value = max_value
            temporary_neighbors = temporary_neighbors.drop(country)
            this_color_countries.append(countries[country])
            continue
        
        if countries[country] in country_color.keys() or countries[country] in get_all_neighbors_from_color(neighbors,this_color_countries):
            temporary_neighbors = temporary_neighbors.drop(country)
            continue
        else:
            this_color_countries.append(countries[country])
            country_color[countries[country]] = color
            temporary_neighbors = temporary_neighbors.drop(country)
        

print(country_color)
