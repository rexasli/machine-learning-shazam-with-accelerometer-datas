/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MY_LIS3DSH.h"
#include "usbd_cdc_if.h"
#include "string.h"
#include <stdio.h>
#include <stdarg.h>
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
char x_arr[32];
char y_arr[32];
char z_arr[32];
char all_arr[100];
void vprint(const char *fmt, va_list argp)
{
    char string[200];
    if(0 < vsprintf(string, fmt, argp)) // build string
    {
        //HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 0xffffff); // send message via UART
    	 CDC_Transmit_FS((uint8_t*)string, strlen(string));
    }
}

void ei_printf(const char *format, ...) {
    va_list myargs;
    va_start(myargs, format);
    vprint(format, myargs);
    va_end(myargs);
}
using namespace ei;

static float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = {64.3030, 166.3883, 949.9591, 56.3030, 150.1942, 934.2856, 52.5455, 153.9223, 943.4694, 53.2727, 181.3010, 947.6326, 46.8485, 171.6311, 913.1020, 33.6364, 140.5243, 945.3061, 20.4242, 145.7670, 1011.4286, 27.3333, 165.2233, 1015.5918, 48.6667, 166.1553, 964.5306, 52.3030, 153.1068, 969.5510, 40.1818, 162.0777, 1006.1632, 40.4242, 169.7670, 984.4897, 53.8788, 149.0291, 960.3673, 61.6364, 127.0097, 901.4694, 46.7273, 135.5146, 925.3469, 37.2727, 169.0680, 958.8979, 51.3333, 195.9806, 944.5714, 67.4545, 177.3398, 927.7959, 56.5455, 159.9806, 967.3469, 25.3939, 168.0194, 997.9591, 5.2727, 170.1165, 1003.1021, 4.1818, 152.1747, 1007.6327, 12.7879, 132.0194, 984.6122, 33.8788, 128.4078, 963.6734, 60.3030, 153.4563, 940.4081, 70.7273, 178.5048, 972.8571, 69.1515, 177.8058, 978.7346, 75.5758, 137.8447, 903.9183, 73.8788, 113.3786, 901.4694, 51.5758, 128.8738, 941.8775, 32.5455, 164.8738, 941.7551, 36.1818, 190.7379, 937.4694, 36.5455, 198.7767, 982.6531, 16.4242, 179.4369, 1062.9796, 8.6667, 127.0097, 1020.9796, 23.3333, 98.0000, 994.0408, 32.1818, 128.6408, 944.2040, 24.9091, 182.9320, 929.5102, 22.2424, 210.5437, 944.2040, 47.5758, 192.2524, 942.2448, 82.3636, 149.8447, 943.9592, 98.8485, 127.0097, 952.8979, 77.5151, 136.3301, 971.5103, 47.3333, 155.9029, 1002.1224, 32.3030, 151.0097, 930.4898, 32.4242, 133.5340, 936.6122, 35.9394, 158.0000, 949.5918, 41.1515, 207.7476, 990.3673, 36.5455, 211.4757, 1006.7754, 29.6364, 170.2330, 1011.9183, 26.0000, 124.5631, 986.8163, 30.1212, 115.4757, 927.0612, 32.4242, 160.6796, 966.7346, 24.0606, 200.9903, 986.6938, 41.1515, 171.6311, 933.0612, 81.0303, 135.7476, 869.3878, 85.5152, 159.9806, 930.1224, 54.2424, 200.4078, 986.4490, 30.1212, 183.3981, 969.0612, 26.3636, 128.8738, 947.2653, 26.8485, 108.9515, 965.1428, 20.6667, 151.9417, 1005.9183, 17.8788, 201.4563, 1005.0613, 31.2121, 187.1262, 978.3673, 54.2424, 140.2913, 940.1632, 64.6667, 129.5728, 900.6122, 56.6667, 166.2718, 908.8163, 40.3030, 198.5437, 923.0204, 24.6667, 194.4660, 970.7755, 39.4545, 163.3592, 973.9591, 70.7273, 145.3010, 957.3061, 69.8788, 160.2136, 945.0612, 20.9091, 190.7379, 990.9795, -23.0909, 188.0582, 1024.1632, -9.1515, 153.4563, 1009.3469, 50.0000, 130.2718, 974.6938, 90.1212, 146.1165, 927.1837, 78.4848, 173.6116, 922.2856, 39.6970, 178.2718, 967.3469, 27.0909, 146.8155, 941.2653, 62.3636, 129.1068, 932.6938, 94.6060, 164.1747, 887.7551, 81.1515, 197.2621, 913.7143, 27.5758, 196.9126, 1034.3264, -20.6667, 157.4175, 1052.9387, -5.8788, 99.8641, 1004.0815, 43.3333, 111.8641, 947.1428, 57.7576, 173.0291, 933.1837, 39.8182, 217.3010, 979.1021, 24.3030, 204.7184, 971.6327, 40.4242, 146.8155, 947.7551, 75.4545, 121.8835, 962.5714, 83.9394, 148.4466, 925.1020, 50.6061, 174.6602, 983.3877, 12.7879, 157.4175, 1014.0000, 25.2727, 111.2815, 978.3673, 71.5758, 117.8058, 930.6122, 88.0606, 173.3786, 902.5714, 53.0303, 199.4757, 954.6122, 5.5152, 177.6893, 1004.6938, -7.2121, 124.4466, 970.7755, 32.9091, 120.9514, 952.8979, 63.0909, 172.6796, 929.8775, 50.1212, 200.5243, 946.7755, 21.3939, 183.8641, 1001.1428, 11.4545, 147.9806, 996.0000, 42.0000, 129.9223, 985.2244, 68.0606, 146.8155, 969.9183, 57.0303, 148.6796, 911.0204, 22.7273, 140.6408, 960.9796, 3.6970, 143.0874, 993.7959, 40.3030, 154.0388, 955.4694, 90.4848, 171.3981, 927.9183, 101.2727, 190.5048, 894.3673, 58.1212, 180.4854, 1011.1837, -6.9697, 134.5825, 1057.2245, -20.1818, 90.6602, 1015.3469, 29.1515, 108.0194, 953.9999, 64.4242, 169.4175, 890.4490, 68.9091, 206.9320, 940.6530, 58.6061, 208.0971, 973.8367, 54.2424, 165.1068, 984.0000, 61.7576, 122.0000, 972.4897, 62.0000, 119.6699, 933.0612, 34.3636, 148.0971, 952.0408, -2.6061, 158.9320, 1027.7142, -0.0606, 134.8155, 992.3265, 61.1515, 135.1651, 922.1632, 96.1818, 180.2524, 883.3469, 74.6061, 214.0388, 950.5714, 31.8182, 196.0971, 1013.8776, 0.6667, 134.3495, 1003.9591, 16.0606, 89.3786, 937.4694, 28.7879, 125.7281, 931.3469, 20.5455, 199.4757, 969.1836, 33.0303, 236.9903, 999.5510, 65.2727, 199.3592, 983.3877, 78.2424, 131.7864, 954.8571, 52.4242, 98.5825, 942.3674, 18.7273, 130.6214, 941.9999, 8.5455, 181.0680, 968.5714, 32.1818, 201.9223, 984.3673, 62.7273, 182.9320, 976.4082, 80.3030, 163.3592, 983.7551, 66.1212, 150.4272, 985.9591, 31.3333, 136.5631, 987.9183, 9.7576, 136.9126, 968.6938, 31.2121, 162.1942, 916.4081, 63.9394, 194.0000, 907.3469, 71.2121, 209.9612, 968.5714, 55.6970, 180.3689, 989.3878, 49.3939, 139.2427, 972.2449, 43.9394, 132.7184, 979.1021, 29.3939, 164.0582, 1002.6122, 23.6970, 173.8447, 981.7959, 39.4545, 145.8835, 933.6735, 65.2727, 137.0291, 944.9387, 76.9091, 167.9029, 925.7143, 61.8788, 200.1747, 917.0204, 31.9394, 198.4272, 929.9999, 4.0606, 167.3204, 965.5103, 3.5758, 140.0582, 1020.6122, 33.6364, 139.0097, 982.7755, 60.4242, 158.8155, 979.7142, 45.3939, 175.7087, 981.7959, 10.3636, 167.3204, 998.4490, 13.6364, 132.1359, 932.3265, 53.5151, 126.3107, 903.1837, 74.8485, 165.9223, 905.1428, 75.0909, 200.6408, 911.5101, 57.6364, 193.1845, 965.8776, 40.1818, 153.2233, 948.3673, 26.6061, 129.6893, 980.3265, 21.2727, 162.0777, 1006.6530, 24.3030, 189.3398, 992.3265, 27.6970, 173.0291, 1000.6530, 20.9091, 141.6893, 1023.4286, 38.1212, 130.7379, 982.7755, 75.9394, 154.2718, 913.7143, 80.1818, 177.2233, 908.3265, 41.8788, 167.7864, 965.5103, 20.3030, 128.4078, 962.4490, 55.9394, 118.3883, 951.5510, 96.0606, 164.7573, 929.0204, 91.8182, 202.2718, 954.9796, 49.1515, 192.4854, 979.5918, 12.6667, 142.5049, 955.8367, -6.1212, 114.5437, 1000.1632, 1.2727, 150.3107, 1029.4286, 38.2424, 187.7087, 950.5714, 60.7879, 178.9709, 909.1837, 39.6970, 156.2524, 992.8163, 19.3333, 142.3883, 996.2449, 58.8485, 154.8544, 972.0000, 94.8485, 173.9612, 892.4081, 82.1212, 168.4854, 937.1020, 26.7273, 136.7961, 1013.1428, -3.5758, 111.2815, 967.8367, 26.1212, 136.9126, 959.7551, 62.0000, 183.6311, 942.1224, 75.4545, 203.2039, 960.4897, 68.3030, 185.0291, 965.2653, 40.7879, 153.1068, 967.1021, -3.6970, 121.0680, 993.4286, -25.3939, 131.4369, 1002.4897, 5.2727, 168.3689, 944.5714, 58.1212, 189.6893, 948.7347, 85.8788, 179.0874, 969.3060, 91.0909, 147.0485, 980.5714, 90.4848, 138.4272, 916.1632, 62.4848, 154.1553, 897.4285, 8.0606, 162.6602, 984.0000, -31.4545, 138.7767, 982.7755, 1.2727, 130.9709, 974.6938, 59.5758, 161.7281, 958.0408, 92.0606, 191.3204, 972.3673, 90.7273, 190.9709, 951.7959, 57.3939, 167.7864, 935.2653, 7.9394, 143.7864, 991.3469, -23.3333, 123.8641, 998.6938, -1.3939, 125.4951, 930.7347, 45.3939, 165.2233, 953.3878, 74.3636, 205.7670, 951.9183, 71.4545, 194.8155, 985.3469, 73.8788, 157.3010, 958.4081, 79.6970, 146.8155, 895.1021, 48.1818, 158.6990, 939.6735, -11.3333, 147.6311, 1006.4081, -41.7576, 127.9417, 1028.4489, 4.1818, 133.7670, 984.1224, 69.1515, 160.6796, 920.6938, 96.9091, 189.4563, 919.8367, 78.4848, 206.2330, 950.9387, 43.2121, 182.5825, 981.7959, 26.6061, 136.6796, 986.8163, 22.7273, 104.9903, 942.3674, 14.6061, 131.3204, 979.7142, 6.6061, 179.2039, 1000.1632, 35.8182, 189.5728, 991.1019, 87.5758, 164.6408, 951.0612, 106.6061, 157.6505, 907.7142, 76.9091, 162.1942, 881.8775, 25.2727, 174.6602, 921.4285, -23.9394, 170.0000, 991.7142, -24.3030, 156.4854, 1029.1837, 27.6970, 142.8544, 983.7551, 82.2424, 152.2913, 987.1836, 106.2424, 180.7184, 973.8367, 81.0303, 177.9223, 933.0612, 38.7273, 152.2913, 895.3469, 6.6061, 145.4175, 937.4694, 3.6970, 166.3883, 945.5510, 33.3939, 176.7573, 954.2449, 67.9394, 172.0971, 994.5305, 77.0303, 166.7379, 1034.2040, 48.1818, 151.7087, 991.5918, 20.7879, 134.0000, 959.7551, 11.3333, 131.6699, 949.3469, 24.3030, 161.9612, 970.2856, 61.7576, 185.6116, 927.4285, 83.9394, 181.6505, 910.5306, 69.1515, 171.8641, 948.6122, 31.9394, 164.9903, 954.9796, 21.0303, 158.3495, 962.9387, 34.3636, 169.5340, 979.7142, 35.0909, 177.5728, 1010.2040, 23.8182, 156.4854, 1002.6122, 21.2727, 120.6019, 1003.2245, 43.8182, 126.4272, 972.1224, 63.8182, 161.3786, 901.9592, 54.2424, 189.1068, 923.6326, 37.1515, 173.3786, 912.0000, 42.3636, 140.2913, 937.8367, 59.6970, 139.1262, 939.7959, 70.7273, 171.8641, 954.4897, 46.1212, 185.2621, 976.4082, 10.0000, 166.8544, 1006.5305, 2.2424, 146.1165, 1017.4286, 23.3333, 152.2913, 987.5510, 34.6061, 165.5728, 986.3265, 35.3333, 161.6116, 958.2857, 42.8485, 146.6990, 939.0612, 55.8182, 160.9126, 948.6122, 56.3030, 169.3010, 950.2041, 55.8182, 149.6116, 979.1021, 61.6364, 129.1068, 924.4897, 63.0909, 143.2039, 914.3265, 51.5758, 189.3398, 965.5103, 38.0000, 219.0485, 1011.1837, 35.2121, 182.2330, 979.3469, 24.1818, 124.5631, 977.3877, -0.7879, 104.5243, 1030.6531, 4.3030, 148.7961, 1005.6735, 47.4545, 192.0194, 935.2653, 74.9697, 202.1553, 929.3877, 59.8182, 173.3786, 961.9591, 29.6364, 129.3398, 981.1836, 29.8788, 105.3398, 945.1836, 58.4848, 135.5146, 945.9183, 79.5758, 180.4854, 943.2244, 76.5454, 185.4951, 920.4489, 58.6061, 153.8058, 931.5918, 32.4242, 135.8641, 950.6938, 9.0303, 156.7184, 982.2858, 16.0606, 188.0582, 982.7755, 23.9394, 188.4078, 1003.7142, 21.5152, 155.9029, 1022.9388, 15.4545, 128.6408, 996.2449, 34.0000, 128.0582, 951.7959, 60.9091, 158.9320, 941.2653, 70.3636, 183.1650, 913.7143, 57.3939, 170.2330, 897.3061, 39.5758, 149.2621, 949.8367, 46.2424, 152.4078, 960.1224, 68.1818, 172.4466, 953.9999, 66.8485, 179.3204, 954.8571, 29.7576, 156.7184, 1008.7346, -4.6667, 133.1845, 1038.1224, 3.2121, 138.0777, 1005.0613, 44.5455, 163.3592, 982.0408, 78.7273, 174.4272, 917.1428, 76.5454, 168.7184, 896.0816, 42.4848, 165.4563, 902.5714, 14.8485, 162.8932, 934.2856, 29.7576, 170.9320, 959.6326, 69.1515, 182.9320, 954.6122, 71.4545, 172.0971, 945.1836, 24.9091, 148.6796, 994.1632, -20.7879, 146.9320, 1056.7347, -2.6061, 161.6116, 1030.7755, 56.1818, 163.1262, 941.8775, 86.2424, 150.1942, 889.2245, 58.0000, 152.7573, 940.2856, 16.3030, 156.7184, 963.4285, 22.6061, 158.0000, 953.5102, 70.1212, 166.1553, 920.3265, 100.9091, 181.4175, 919.8367, 80.9091, 179.3204, 951.5510, 39.8182, 156.4854, 997.1021, 11.3333, 139.7087, 985.2244, 5.3939, 158.1165, 1000.4081, 5.5152, 185.9612, 1010.9388, 18.7273, 170.9320, 1002.2449, 47.6970, 134.3495, 961.2245, 67.9394, 145.1845, 910.2857, 53.0303, 195.0485, 947.3878, 23.9394, 210.3107, 976.1632, 21.7576, 159.1650, 984.6122, 47.3333, 104.4078, 973.9591, 73.7576, 111.3981, 979.3469, 81.2727, 158.1165, 961.5918, 67.8182, 188.1747, 918.2449, 46.1212, 172.4466, 905.8776, 28.1818, 156.2524, 935.0204, 20.5455, 163.9417, 976.6531, 24.1818, 173.9612, 1002.6122, 29.0303, 167.9029, 1005.0613, 29.7576, 150.8932, 993.0612, 30.1212, 146.5825, 979.3469, 42.4848, 155.3204, 967.7142, 60.5454, 170.1165, 967.7142, 70.2424, 169.6505, 928.6531, 63.3333, 149.2621, 872.8163, 43.0909, 154.1553, 916.6531, 17.3939, 184.0971, 981.0612, 15.6970, 187.7087, 1000.5305, 40.3030, 139.2427, 972.1224, 52.5455, 101.1456, 965.7551, 39.4545, 140.6408, 1006.1632, 22.7273, 197.1456, 1002.3673, 22.9697, 194.5825, 985.5918, 46.8485, 131.9029, 931.9592, 71.8182, 106.8544, 915.6735, 72.4242, 155.4369, 906.0000, 50.4848, 204.2524, 913.9592, 28.7879, 204.0194, 970.6531, 30.0000, 164.6408, 990.1224, 51.8182, 125.6116, 947.3878, 48.1818, 129.4563, 944.2040, 13.0303, 182.2330, 1019.7551, -6.7273, 215.3204, 1053.4286, 17.6364, 168.7184, 983.2653, 43.6970, 101.2621, 935.8775, 52.0606, 107.9029, 931.7143, 47.8182, 173.0291, 931.7143, 49.5151, 220.5631, 984.1224, 63.9394, 188.0582, 966.0000, 80.5454, 115.9417, 895.1021, 80.1818, 101.9612, 884.0816, 49.5151, 161.0291, 957.4285, 20.0606, 218.3495, 1037.1428, 15.4545, 207.3981, 1029.5510, 31.6970, 135.3981, 950.6938, 38.0000, 95.5534, 908.8163, 17.2727, 151.4757, 998.0815, 5.0303, 228.8349, 1046.2040, 25.8788, 217.1845, 995.7551, 58.4848, 136.0971, 929.1428, 74.1212, 83.7864, 890.8163, 72.4242, 123.7476, 909.4285, 56.9091, 196.7961, 947.9999, 39.3333, 222.7767, 1009.9591, 45.6364, 164.6408, 962.4490, 66.6061, 97.8835, 892.0408, 54.0000, 113.7281, 960.8571, 27.0909, 182.6990, 1018.4081, 20.6667, 218.5825, 1039.8367, 41.8788, 170.0000, 960.8571, 45.3939, 96.1359, 873.5510, 17.6364, 100.3301, 927.0612, 0.1818, 187.7087, 973.5918, 22.9697, 254.3495, 1023.6735, 66.0000, 226.7379, 1000.1632, 79.5758, 134.4660, 930.6122, 66.0000, 65.1456, 896.2040, 40.9091, 103.0097, 926.2040, 19.9394, 189.5728, 1016.8164, 30.6061, 229.6505, 1034.2040, 58.4848, 165.6893, 906.2449, 71.5758, 98.3495, 878.2041, 55.9394, 124.0971, 953.2653, 36.7879, 197.1456, 1016.0815, 39.4545, 221.6116, 1032.8572, 43.4545, 148.2136, 928.8979, 33.5152, 76.9126, 894.3673, 16.0606, 115.9417, 951.5510, 17.7576, 213.4563, 1004.4490, 42.7273, 254.8155, 1028.4489, 66.9697, 200.0582, 954.2449, 63.5758, 112.6796, 913.1020, 48.1818, 87.9806, 911.8776, 31.8182, 152.2913, 949.4694, 22.2424, 217.6505, 1019.7551, 23.6970, 200.4078, 992.8163, 47.2121, 120.4854, 898.0408, 72.3030, 106.9709, 907.5918, 60.6667, 182.2330, 953.9999, 33.2727, 240.1359, 1018.2856, 20.3030, 205.3010, 1010.4489, 32.5455, 109.8835, 923.6326, 45.1515, 72.6019, 886.4081, 28.0606, 141.9223, 954.7347, 4.5455, 226.3883, 1021.9591, 28.1818, 229.6505, 1010.5714, 73.6364, 170.6990, 934.0408, 99.5758, 119.3204, 881.8775, 72.9091, 130.6214, 922.8979, 21.0303, 175.5922, 976.5306, -11.6970, 193.1845, 989.3878, 6.9697, 158.4660, 974.5714, 59.3333, 137.4951, 964.0408, 91.9394, 164.6408, 964.6531, 75.0909, 187.5922, 979.3469, 33.8788, 156.3689, 986.9387, 28.7879, 107.6699, 908.4489, 52.0606, 122.2330, 879.7959, 43.2121, 190.3883, 963.5510, 12.6667, 220.5631, 985.1019, 12.5455, 181.7670, 1016.5713, 54.4848, 139.1262, 986.3265, 87.2121, 128.8738, 981.0612, 74.6061, 151.1262, 976.1632, 33.1515, 166.8544, 929.9999, 15.6970, 158.3495, 926.9387, 32.1818, 158.0000, 934.5306, 62.7273, 173.2621, 934.4081, 74.8485, 182.0000, 947.1428, 51.2121, 171.3981, 985.4694, 11.0909, 148.7961, 1014.6122, 2.0000, 126.0777, 1001.8776, 29.6364, 130.3883, 970.4081, 54.2424, 158.8155, 962.3265, 59.2121, 183.7476, 968.2040, 52.3030, 177.6893, 945.3061, 41.3939, 146.2330, 931.8367, 35.2121, 132.1359, 930.9796, 48.5455, 155.3204, 964.5306, 70.1212, 182.2330, 969.7959, 66.3636, 167.3204, 931.4694, 26.4848, 134.5825, 983.7551, -4.5455, 135.7476, 995.5103, 13.5152, 176.2912, 1007.7551, 46.6061, 186.1942, 972.6122, 52.5455, 147.6311, 949.1020, 24.0606, 118.6214, 956.5714, 15.4545, 152.5243, 975.4286, 52.3030, 193.3010, 990.3673, 97.2727, 183.9806, 935.6326, 86.8485, 131.4369, 885.9183, 34.7273, 107.4369, 991.3469, 7.8182, 136.6796, 991.1019, 46.2424, 184.6796, 942.4898, 85.5152, 209.7281, 900.0000, 80.3030, 188.2912, 928.8979, 23.3333, 135.7476, 969.9183, -16.1818, 114.3107, 1004.3264, 16.4242, 167.3204, 1011.3062, 67.2121, 209.2621, 998.0815, 78.3636, 188.5243, 944.0816, 39.2121, 132.8349, 932.5714, -5.7576, 102.8932, 974.5714, 3.4545, 145.4175, 1005.5510, 59.3333, 196.6796, 949.1020, 104.9091, 203.2039, 850.5306, 94.4848, 177.9223, 937.7142, 37.0303, 131.2039, 995.1428, 7.6970, 109.8835, 979.3469, 39.6970, 159.8641, 965.8776, 54.9697, 201.6893, 927.5510, 19.8182, 174.6602, 949.9591, -13.2727, 127.1262, 969.4285, 20.3030, 143.7864, 974.9387, 79.0909, 193.5340, 958.8979, 96.5454, 215.9029, 922.7755, 52.4242, 178.5048, 927.6735, -4.3030, 114.5437, 970.7755, -23.6970, 103.5922, 1027.4694, 27.8182, 161.6116, 987.1836, 84.1818, 205.7670, 921.4285, 100.7879, 207.3981, 908.0816, 62.4848, 160.4466, 971.6327, 5.8788, 101.2621, 991.8367, 11.3333, 114.6602, 969.4285, 50.4848, 173.4951, 925.8367, 57.8788, 190.9709, 896.5714, 25.8788, 157.5340, 948.1224, 10.6061, 137.2621, 995.3878, 59.0909, 170.8155, 1000.2856, 106.0000, 204.1359, 957.6735, 87.9394, 186.3107, 913.5918, 21.5152, 128.8738, 957.0612, -27.0909, 86.4660, 990.9795, -9.6364, 126.8932, 991.1019, 49.8788, 194.0000, 958.0408, 86.3636, 223.5922, 930.3673, 76.9091, 189.2233, 952.6530, 41.5151, 127.0097, 978.8571, 20.3030, 95.5534, 1006.4081, 40.3030, 145.3010, 959.7551, 54.9697, 180.0194, 890.9387, 28.3030, 163.2427, 898.1633, -4.6667, 135.9806, 986.3265, 22.3636, 161.6116, 1019.2653, 81.7576, 196.6796, 946.5306, 98.8485, 209.2621, 915.9183, 53.1515, 171.7476, 939.1836, -9.7576, 108.8349, 1013.3877, -33.0303, 84.9515, 961.4694, -2.1212, 139.2427, 963.4285, 45.2727, 207.1650, 935.9999, 79.8182, 235.0097, 929.6326, 88.0606, 211.2427, 966.2449, 58.4848, 155.5534, 975.6735, 4.9091, 97.8835, 991.1019, -17.1515, 93.5728, 986.8163, 14.7273, 145.4175, 964.7755, 73.0303, 186.6602, 919.2244, 101.0303, 199.4757, 910.4081, 79.5758, 190.9709, 937.4694, 38.6061, 170.6990, 970.7755, 9.6364, 147.8641, 1000.2856, 7.8182, 139.4757, 1005.5510, 27.8182, 142.8544, 1028.3264, 58.8485, 147.5146, 981.3062, 80.4242, 152.2913, 932.3265, 71.5758, 176.0582, 939.4286, 40.4242, 195.3981, 941.3878, 24.4242, 176.9903, 915.6735, 35.9394, 140.2913, 904.4081, 43.2121, 145.8835, 969.4285, 37.7576, 173.0291, 988.8978, 34.3636, 176.1747, 988.1632, 37.0303, 157.4175, 1006.0408, 43.0909, 145.8835, 994.6530, 40.7879, 150.1942, 978.6122, 36.7879, 157.4175, 950.6938, 43.4545, 161.2621, 918.3673, 52.9091, 166.3883, 944.9387, 58.8485, 169.5340, 930.6122, 64.6667, 158.8155, 915.5510, 64.1818, 160.2136, 959.7551, 53.2727, 185.1456, 987.7959, 31.3333, 192.8349, 1003.7142, 6.9697, 154.1553, 987.1836, 2.0000, 122.0000, 1002.8571, 23.5758, 142.6214, 992.6938, 55.0909, 187.8252, 961.3469, 62.9697, 193.5340, 922.1632, 38.1212, 155.7864, 957.4285, 18.3636, 124.5631, 956.4490, 45.1515, 137.6116, 952.8979, 80.7879, 170.6990, 950.5714, 86.6061, 178.2718, 936.9796, 62.9697, 154.9709, 962.3265, 35.2121, 133.5340, 942.2448, 17.6364, 141.9223, 941.0204, 12.0606, 182.5825, 1001.3878, 22.6061, 201.5728, 989.6327, 47.2121, 168.1359, 986.9387, 55.9394, 134.0000, 962.6938, 27.0909, 134.5825, 975.4286, 3.8182, 159.7476, 995.6327, 13.1515, 175.3592, 943.7142, 50.4848, 166.1553, 947.1428, 89.8788, 156.9514, 948.7347, 94.3636, 153.2233, 911.1428, 67.3333, 161.8447, 944.3265, 30.0000, 169.7670, 956.4490, 7.8182, 150.7767, 948.6122, 20.1818, 140.5243, 998.2040, 44.4242, 150.0777, 961.3469, 63.4545, 176.2912, 985.7142, 47.6970, 199.8252, 983.8776, 12.1818, 177.6893, 964.4081, 4.1818, 134.6990, 980.9387, 24.6667, 130.1553, 949.4694, 50.4848, 161.3786, 952.8979, 66.0000, 186.1942, 972.7346, 63.6970, 169.5340, 957.1837, 59.8182, 144.1359, 977.6327, 58.3636, 141.9223, 943.3469, 45.2727, 154.3883, 957.5510, 37.1515, 167.0874, 976.0408, 36.7879, 166.5049, 955.5918, 38.0000, 152.6408, 987.1836, 33.7576, 154.8544, 975.0612, 27.5758, 160.0971, 943.2244, 29.5151, 181.7670, 998.8163, 32.0606, 188.5243, 977.8776, 46.6061, 152.1747, 957.3061, 61.7576, 132.6019, 933.1837, 52.5455, 162.3107, 956.8163, 34.8485, 193.1845, 978.2449, 31.9394, 169.7670, 942.8571, 45.5151, 134.1165, 951.9183, 58.1212, 143.2039, 981.0612, 48.3030, 172.7961, 947.5102, 28.1818, 172.0971, 963.9183, 16.5455, 168.3689, 982.5306, 27.5758, 160.3301, 975.4286, 54.9697, 156.3689, 945.1836, 59.0909, 153.2233, 938.6938, 42.6061, 178.5048, 1010.8163, 36.7879, 195.5146, 985.8367, 48.5455, 157.3010, 942.1224, 50.8485, 111.7476, 915.7959, 39.6970, 132.4854, 928.7755, 33.3939, 190.6214, 925.9592, 39.8182, 226.5048, 1019.2653, 46.7273, 191.0874, 1017.0613, 57.1515, 125.8447, 947.7551, 50.6061, 107.0874, 918.8571, 26.8485, 162.1942, 984.3673, 7.3333, 212.4078, 1016.9388, 21.6364, 194.0000, 965.0204, 50.1212, 137.6116, 968.8163, 61.1515, 108.2524, 945.7959, 49.6364, 137.0291, 953.1428, 36.1818, 187.5922, 970.1632, 45.6364, 189.9223, 916.7755, 65.2727, 149.8447, 904.4081, 61.7576, 140.9903, 934.5306, 34.4848, 177.1068, 970.0408, 5.8788, 194.0000, 1029.1837, 3.6970, 153.1068, 995.1428, 30.2424, 108.1359, 1008.3673, 69.7576, 129.2233, 975.1836, 74.2424, 169.1845, 892.4081, 41.0303, 178.2718, 932.0816, 11.4545, 171.8641, 936.6122, 33.6364, 156.6019, 937.2244, 82.8485, 167.3204, 920.4489, 89.2727, 182.8155, 931.1020, 42.7273, 182.8155, 1015.8367, -5.7576, 158.4660, 1028.4489, -5.8788, 123.0485, 1007.7551, 34.6061, 129.1068, 999.0613, 66.8485, 163.9417, 967.9591, 70.3636, 179.7864, 931.5918, 49.8788, 157.7670, 934.0408, 37.3939, 132.0194, 929.3877, 57.3939, 137.2621, 903.6734, 84.9091, 172.5631, 863.6326, 65.6364, 201.6893, 927.6735, 17.0303, 193.7670, 1036.1632, 1.5152, 155.2039, 1039.4694, 37.3939, 130.0388, 1002.3673, 59.5758, 148.6796, 980.8163, 34.7273, 179.2039, 975.9183, -7.4545, 172.0971, 975.6735, -3.8182, 140.9903, 973.2245, 49.5151, 132.9514, 955.5918, 96.0606, 159.5146, 908.4489, 105.1515, 186.3107, 887.1428, 71.8182, 188.8738, 965.8776, 24.3030, 154.6214, 999.1837, 20.1818, 120.6019, 962.6938, 50.9697, 138.6602, 930.4898, 55.5758, 190.9709, 947.7551, 22.4848, 202.1553, 975.1836, -10.6061, 162.0777, 1015.8367, 4.5455, 120.7184, 997.2245, 49.6364, 137.0291, 973.1021, 78.4848, 180.3689, 891.6734, 64.6667, 197.6116, 926.6938, 26.6061, 170.8155, 991.1019, 15.6970, 120.9514, 982.0408, 57.7576, 120.8349, 951.3061, 87.6970, 163.8252, 915.6735, 74.4848, 188.1747, 940.1632, 31.8182, 171.9806, 974.3264, 11.8182, 134.2330, 1001.3878, 28.7879, 117.1068, 1002.7346, 60.1818, 144.6019, 914.5714, 51.3333, 181.6505, 905.3877, 14.9697, 191.7864, 991.1019, -6.4848, 169.7670, 1010.0815, 33.3939, 142.3883, 971.2653, 75.0909, 152.7573, 924.7347, 70.7273, 173.0291, 951.1836, 30.8485, 174.4272, 970.1632, 7.0909, 155.9029, 990.2449, 35.0909, 139.5922, 997.1021, 81.7576, 137.9612, 941.5102, 86.8485, 145.4175, 875.9999, 50.4848, 158.0000, 928.4081, 7.3333, 174.6602, 1041.9183, 18.9697, 160.7961, 983.3877, 62.6061, 142.7379, 918.6122, 62.9697, 145.6505, 935.2653, 22.0000, 162.7767, 1005.3062, -8.1818, 182.8155, 1025.3877, 15.0909, 170.8155, 977.3877, 60.3030, 148.9126, 1000.4081, 83.8182, 139.2427, 904.4081, 62.2424, 147.2816, 887.7551, 16.4242, 165.1068, 984.6122, -3.2121, 173.7281, 1000.2856, 28.6667, 151.5922, 939.9183, 68.9091, 145.0680, 893.6326, 69.3939, 164.9903, 965.2653, 38.8485, 189.8058, 1036.0408, 14.8485, 163.9417, 980.0815, 33.3939, 118.0388, 963.3061, 63.2121, 127.0097, 969.1836, 56.9091, 173.0291, 918.8571, 30.2424, 192.6019, 958.0408, 5.2727, 175.8252, 999.1837, 30.9697, 134.5825, 952.7755, 70.7273, 130.3883, 894.7346, 68.5455, 171.6311, 958.1633, 40.5455, 201.4563, 1015.5918, 12.7879, 169.7670, 992.0815, 24.9091, 114.0777, 941.8775, 49.5151, 135.1651, 962.3265, 49.0303, 205.3010, 966.9796, 38.0000, 234.5437, 959.6326, 28.9091, 184.9126, 1012.7754, 22.2424, 100.4466, 983.3877, 40.7879, 73.3010, 981.3062, 69.1515, 142.8544, 942.7347, 77.5151, 204.6019, 881.1429, 64.5455, 211.5922, 889.9591, 37.5152, 189.1068, 949.3469, 21.5152, 163.9417, 981.3062, 34.1212, 142.8544, 1000.4081, 50.0000, 137.2621, 1007.6327, 43.8182, 148.9126, 1001.6327, 30.1212, 160.9126, 984.8571, 33.8788, 159.0485, 946.4081, 52.0606, 154.9709, 904.7755, 65.8788, 162.6602, 923.2653, 61.1515, 176.9903, 921.7959, 47.4545, 172.0971, 927.1837, 37.5152, 168.2524, 979.7142, 35.0909, 163.2427, 975.6735, 25.0303, 141.5728, 962.9387, 6.2424, 126.0777, 986.3265, 1.7576, 147.7476, 1014.8572, 40.9091, 188.0582, 1005.3062, 87.4545, 195.3981, 951.5510, 94.7273, 157.6505, 899.1428, 55.8182, 118.2718, 928.2856, 11.6970, 121.4175, 939.7959, 14.9697, 156.1359, 891.4285, 54.4848, 194.4660, 938.6938, 83.8182, 209.3786, 970.8979, 71.5758, 164.8738, 983.7551, 36.1818, 109.4175, 952.4081, 5.3939, 128.4078, 1029.9183, 8.1818, 193.4175, 1014.2449, 30.0000, 198.1942, 945.1836, 32.9091, 138.0777, 942.6122, 20.4242, 104.2913, 968.5714, 31.5758, 148.4466, 993.5510, 81.8788, 196.9126, 937.1020, 115.0909, 198.5437, 919.8367, 101.5151, 158.6990, 927.6735, 54.6061, 114.4272, 887.6326, 5.6364, 116.7573, 962.6938, -14.3636, 170.1165, 1011.5510, 16.4242, 213.2233, 998.6938, 71.0909, 202.5048, 964.7755, 81.5151, 156.9514, 942.3674, 34.1212, 125.1456, 1020.9796, -16.3030, 133.1845, 1005.3062, -6.6061, 165.9223, 993.6735, 50.6061, 175.4757, 944.0816, 86.0000, 144.1359, 883.3469, 68.9091, 126.0777, 922.5306, 44.3030, 160.4466, 969.1836, 65.6364, 200.5243, 967.9591, 96.9091, 186.6602, 900.3673, 75.4545, 142.6214, 883.5918, 5.6364, 110.8155, 1008.7346, -42.1212, 122.6990, 1047.5510, -14.3636, 161.0291, 987.7959, 46.2424, 194.2330, 948.8571, 80.1818, 203.3204, 943.4694, 60.7879, 169.7670, 937.8367

};

int get_feature_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_CRC_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

LIS3DSH_DataScaled myData;
uint8_t trex=0;
//char charbuff[50];
//int pressed=0;

using namespace std;
bool featuresarrayisready=false;
int countt=0;
void combiner(){
			strcat(all_arr,x_arr);
			strcat(all_arr,y_arr);
			strcat(all_arr,z_arr);
			}
void cleaner(){
	for(int i=0;i<100;i++){
		x_arr[i]='\0';
		y_arr[i]='\0';
		z_arr[i]='\0';
		all_arr[i]='\0';
}}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	LIS3DSH_InitTypeDef myAccConfigDef;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USB_DEVICE_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */
	myAccConfigDef.dataRate = LIS3DSH_DATARATE_800;
	myAccConfigDef.fullScale = LIS3DSH_FULLSCALE_4;
	myAccConfigDef.enableAxes = LIS3DSH_XYZ_ENABLE;
	myAccConfigDef.interruptEnable = true;
	//myAccConfigDef.antiAliasingBW = LIS3DSH_FILTER_BW_50;
	LIS3DSH_Init(&hspi1, &myAccConfigDef);

	LIS3DSH_X_calibrate(-1000.0, 980.0);
	LIS3DSH_Y_calibrate(-1020.0, 1040.0);
	LIS3DSH_Z_calibrate(-920.0, 1040.0);
	signal_t signal;
	  	signal.total_length = sizeof(features) / sizeof(features[0]);
	  	signal.get_data = &get_feature_data;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  for(;featuresarrayisready==false;)
		{
		  	if(trex==1){
		  		trex=0;
		  		myData = LIS3DSH_GetDataScaled();
		  		  	features[3*countt]=myData.x;
		  		  	features[3*countt+1]=myData.y;
		  		  	features[3*countt+2]=myData.z;
		  		  	countt++;
			  if(countt==EI_CLASSIFIER_RAW_SAMPLE_COUNT){
						countt=0;
						featuresarrayisready=true;
					}


		  	}

		}
	  ei_impulse_result_t result = { 0 };
	  	      EI_IMPULSE_ERROR res = run_classifier(&signal, &result, true);
	  	      ei_printf("run_classifier returned: %d\n", res);

	  	      ei_printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n",
	  	          result.timing.dsp, result.timing.classification, result.timing.anomaly);

	  	      // print the predictions
	  	      ei_printf("[");
	  	      for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
	  	    	  ei_printf_float(result.classification[ix].value);
	  	  #if EI_CLASSIFIER_HAS_ANOMALY == 1
	  	          ei_printf(", ");
	  	  #else
	  	          if (ix != EI_CLASSIFIER_LABEL_COUNT - 1) {
	  	              ei_printf(", ");
	  	          }
	  	  #endif
	  	      }
	  	  #if EI_CLASSIFIER_HAS_ANOMALY == 1
	  	      ei_printf_float(result.anomaly);
	  	  #endif
	  	      ei_printf("]\n\n\n");



	  HAL_Delay(1500);
	  featuresarrayisready=false;

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MEMS_CS_GPIO_Port, MEMS_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : MEMS_CS_Pin */
  GPIO_InitStruct.Pin = MEMS_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MEMS_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PE0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0)){
				trex = 1;
		}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
