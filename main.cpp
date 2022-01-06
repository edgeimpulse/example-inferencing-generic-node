/** Copyright © 2021 The Things Industries B.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file main.c
 *
 * @copyright Copyright (c) 2021 The Things Industries B.V.
 *
 */

#include "app.h"
#include "classifier/ei_run_classifier.h"

UART_HandleTypeDef huart2;

static void SystemClock_Config(void);
static void Error_Handler(void);
static void MX_USART2_UART_Init(void);

void ei_printf(const char *format, ...);
void vprint(const char *fmt, va_list argp);
void run_inference();

float features[] = {
		// copy and replace raw features here (for example from the 'Live classification' page)
		-4.7748, -0.1832, 7.5420, -5.9158, -0.1246, 8.7138, -5.7701, -0.0712, 8.1296, -5.8911, 0.0707, 8.4771, -5.8716, -0.0454, 8.9153, -6.2134, 0.3014, 9.0653, -5.8627, -0.3461, 8.8752, -5.6839, -1.0136, 8.8057, -5.3633, -0.2126, 8.4734, -5.1094, -0.2641, 9.0683, -5.0459, -0.5006, 8.7697, -5.1125, -0.3578, 8.7740, -5.0834, -0.2541, 8.9325, -5.0232, -0.6613, 8.9310, -5.3456, -0.3904, 9.0583, -5.2083, -0.0567, 8.3045, -5.3051, -0.2952, 8.5320, -5.4853, -0.0309, 8.6330, -5.1833, -0.1056, 8.5085, -5.4898, -0.4242, 8.3534, -5.5421, 0.0274, 8.5578, -5.4650, -0.0450, 8.8450, -5.3482, -0.0717, 8.6684, -5.3494, 0.0505, 8.3679, -5.2641, 0.0678, 8.3090, -5.2381, -0.1303, 8.4965, -5.2689, 0.1007, 8.8114, -5.1680, -0.0445, 9.0311, -5.3312, 0.1983, 9.0977, -5.7772, -0.0711, 9.4342, -6.2392, -0.2101, 9.5509, -5.3701, -0.1991, 9.0347, -5.1114, -0.5984, 8.1236, -5.5179, -0.3531, 8.9569, -5.5152, -0.5603, 8.7327, -5.2385, -0.6191, 8.6405, -5.2161, -0.5678, 8.9028, -5.7633, -0.5389, 9.1267, -5.5249, -0.3405, 8.7136, -5.5063, -0.1839, 8.3565, -5.5651, -0.0126, 8.7137, -5.5261, -0.0830, 8.4667, -5.3887, -0.4732, 8.1905, -5.4625, -0.3390, 8.1702, -5.3715, -0.1572, 8.4680, -5.5461, -0.2227, 8.8910, -5.3446, -0.1758, 8.4325, -5.2749, -0.4460, 8.5357, -5.3563, -0.3875, 8.4830, -5.6548, -0.1000, 8.7550, -5.4737, 0.0349, 8.7908, -5.5689, -0.1876, 8.8200, -5.3951, -0.3339, 8.9170, -5.6010, -0.5145, 8.4767, -5.2390, -0.1386, 8.2043, -5.4066, -0.2388, 8.7727, -5.7684, -0.2350, 8.9297, -5.4921, -0.0466, 8.3851, -5.6943, -0.2404, 8.9322, -5.5522, -0.1557, 8.8674, -5.6554, -0.4252, 8.7600, -5.6100, -0.0742, 8.7328, -5.5641, -0.4113, 8.5806, -5.4635, -0.2739, 8.6784, -5.3932, -0.0930, 8.6850, -5.3459, -0.3338, 8.3688, -5.2544, 0.0672, 8.4388, -5.4658, -0.0304, 8.3462, -5.4146, -0.1629, 8.8244, -5.6533, -0.1796, 8.6408, -5.6125, 0.0608, 8.5447, -5.4174, -0.1698, 8.5850, -5.2031, -0.1158, 8.2647, -5.2693, -0.3447, 8.5187, -5.5409, -0.0715, 8.5780, -5.5897, 0.2950, 8.8438, -5.4636, -0.2356, 8.6434, -5.3900, -0.2064, 8.7172, -5.2671, -0.2065, 8.7421, -5.5003, -0.1824, 8.8604, -5.5017, -0.0757, 8.7973, -5.4658, -0.1047, 8.7260, -5.4405, -0.1132, 8.7275, -5.5124, -0.0105, 8.8853, -5.5812, -0.2827, 8.6648, -5.4700, -0.0960, 8.6089, -5.3998, 0.2429, 8.6019, -5.3875, 0.1866, 8.5915, -5.5117, -0.0525, 8.6253, -5.4418, -0.0020, 8.7874, -5.5147, -0.0442, 8.8704, -5.4229, -0.1101, 8.7135, -5.2433, -0.2736, 8.6262, -5.3623, -0.2927, 8.7742, -5.3489, -0.1644, 8.5028, -5.2892, -0.3493, 8.5988, -5.5563, -0.0080, 8.7930, -5.5134, -0.1475, 8.7586, -5.4810, -0.1157, 9.0637, -5.6424, -0.0685, 8.7863 };

/**
 * --------------------------- GENERAL FUNCTIONS ----------------------------------
 */

/**
 * @brief System Clock Configuration
 * @return None
 */
static void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE
			| RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3 | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @return None
 */
static void Error_Handler(void) {
	while (1) {
	}
}



/*
 * --------------------------- Edge Impulse functions ---------------------------
 */

/**
 * Low-level print function that uses UART to print status messages.
 */
void vprint(const char *fmt, va_list argp) {
	char string[200];
	if (0 < vsprintf(string, fmt, argp)) // build string
			{
		HAL_UART_Transmit(&huart2, (uint8_t*) string, strlen(string), 0xffffff);
	}
}

/**
 * Wrapper for vprint. Use this like you would printf to print messages to the serial console.
 */
void ei_printf(const char *format, ...) {
	va_list myargs;
	va_start(myargs, format);
	vprint(format, myargs);
	va_end(myargs);
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief      Copy raw feature data in out_ptr
 *             Function called by inference library
 *
 * @param[in]  offset   The offset
 * @param[in]  length   The length
 * @param      out_ptr  The out pointer
 *
 * @return     0
 */
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
	memcpy(out_ptr, features + offset, length * sizeof(float));
	return 0;
}

/**
 * @brief      Run inference
 *
 * @param[in]  offset   The offset
 * @param[in]  length   The length
 * @param      out_ptr  The out pointer
 *
 * @return     0
 */

void run_inference() {
	ei_printf("Edge Impulse standalone inferencing\n");

	if (sizeof(features) / sizeof(float) != EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
		ei_printf(
				"The size of your 'features' array is not correct. Expected %d items, but had %u\n",
				EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE,
				sizeof(features) / sizeof(float));
		return;
	}

	ei_impulse_result_t result = { 0 };

	// the features are stored into flash, and we don't want to load everything into RAM
	ei_printf("Getting signal...");
	signal_t features_signal;
	features_signal.total_length = sizeof(features) / sizeof(features[0]);
	features_signal.get_data = &raw_feature_get_data;
	ei_printf("Done\n");

	ei_printf("Run classifier...");
	// Feed signal to the classifier
	EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false /* debug */);
	ei_printf("Done\n");

	// Returned error variable "res" while data object.array in "result"
	ei_printf("run_classifier returned: %d\n", res);
	if (res != 0)
		return;

	// print the predictions
	ei_printf(
			"Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n",
			result.timing.dsp, result.timing.classification,
			result.timing.anomaly);
	for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
		int val = (int)((result.classification[ix].value * 100) + 0.5);
		ei_printf("    %s: \t %i%%\r\n", result.classification[ix].label, val);
	}
#if EI_CLASSIFIER_HAS_ANOMALY == 1
	  ei_printf("    anomaly score: %f\r\n", result.anomaly);
	#endif

}

int main(void) {
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	SystemClock_Config();
	MX_USART2_UART_Init();


//	GNSE_BSP_LED_Init(LED_RED);
//	GNSE_BSP_LED_Init(LED_GREEN);
//	GNSE_BSP_LED_Init(LED_BLUE);

	// Say some stuff
	ei_printf("Inferencing settings:\r\n");
	ei_printf("\tInterval: %d ms.\r\n", EI_CLASSIFIER_INTERVAL_MS);
	ei_printf("\tFrame size: %d\r\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
	ei_printf("\tSample length: %d ms.\r\n",
	EI_CLASSIFIER_RAW_SAMPLE_COUNT / 16);
	ei_printf("\tNo. of classes: %d\r\n\n",
			sizeof(ei_classifier_inferencing_categories)
					/ sizeof(ei_classifier_inferencing_categories[0]));

	run_inference();

	while (1) {
//		GNSE_BSP_LED_Toggle(LED_RED);
//		HAL_Delay(100);
//		GNSE_BSP_LED_Toggle(LED_GREEN);
//		HAL_Delay(100);
//		GNSE_BSP_LED_Toggle(LED_BLUE);
//		HAL_Delay(100);
	}
	return 0;
}

