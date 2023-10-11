# defining the libraries 
import numpy as np 
import matplotlib.pyplot as plt 
import pandas as pd 

# No of Data points 
N = 500

# initializing random values 
part_a_enabled = [18.62, 18.50, 18.64, 18.69, 18.59, 18.51, 18.46, 18.53, 18.62, 18.66, 18.77, 18.66, 18.64, 18.71, 18.62, 18.60, 18.71, 18.50, 18.59, 18.57, 18.59, 18.62, 18.62, 18.55]
part_a_disabled = [12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00]

part_b_enabled = [6.02, 6.49, 6.60, 6.58, 5.84, 6.53, 6.47, 6.67, 6.40, 6.54, 6.33, 6.78, 6.18, 6.54, 6.72, 5.82, 7.08, 6.65, 6.80, 6.17, 6.89, 6.63, 6.45, 6.65]
part_b_disabled = [6.57, 6.85, 6.73, 6.73, 6.63, 7.12, 7.17, 6.97, 6.74, 6.95, 6.79, 6.67, 6.76, 6.75, 6.58, 7.05, 7.26, 6.86, 6.98, 6.78, 7.12, 6.62, 6.97, 6.98]

# getting data of the histogram 
a_en_count, a_en_bins_count = np.histogram(part_a_enabled, bins=24)
a_dis_count, a_dis_bins_count = np.histogram(part_a_disabled, bins=24)

b_en_count, b_en_bins_count = np.histogram(part_b_enabled, bins=24)
b_dis_count, b_dis_bins_count = np.histogram(part_b_disabled, bins=24)

# finding the PDF of the histogram using count values 
a_en_pdf = a_en_count / sum(a_en_count)
a_dis_pdf = a_dis_count / sum(a_dis_count)

b_en_pdf = b_en_count / sum(b_en_count)
b_dis_pdf = b_dis_count / sum(b_dis_count)

# using numpy np.cumsum to calculate the CDF 
# We can also find using the PDF values by looping and adding 
a_en_cdf = np.cumsum(a_en_pdf)
a_dis_cdf = np.cumsum(a_dis_pdf)

b_en_cdf = np.cumsum(b_en_pdf)
b_dis_cdf = np.cumsum(b_dis_pdf)

# plotting PDF and CDF
f = plt.figure(1)
plt.plot(a_en_bins_count[1:], a_en_cdf, color="blue", label="(a) Cache Enabled") 
plt.legend()

g = plt.figure(2)
plt.plot(a_dis_bins_count[1:], a_dis_cdf, color="red", label="(a) Cache Disabled") 
plt.legend()

f = plt.figure(3)
plt.plot(b_en_bins_count[1:], b_en_cdf, color="blue", label="(b) Cache Enabled") 
plt.legend()

g = plt.figure(4)
plt.plot(b_dis_bins_count[1:], b_dis_cdf, color="red", label="(b) Cache Disabled") 
plt.legend()

plt.show(block=True)