/***************************************************
Copyright (c) 2017 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/
 
#include "DoubleEMAFilterLib.h"

int values[] = { 7729, 7330, 10075, 10998, 11502, 11781, 12413, 12530, 14070, 13789, 18186, 14401, 16691, 16654, 17424, 21104, 17230, 20656, 21584, 21297, 19986, 20808, 19455, 24029, 21455, 21350, 19854, 23476, 19349, 16996, 20546, 17187, 15548, 9179, 8586, 7095, 9718, 5148, 4047, 3873, 4398, 2989, 3848, 2916, 1142, 2427, 250, 2995, 1918, 4297, 617, 2715, 1662, 1621, 960, 500, 2114, 2354, 2900, 4878, 8972, 9460, 11283, 16147, 16617, 16778, 18711, 22036, 28432, 29756, 24944, 27199, 27760, 30706, 31671, 32185, 32290, 30470, 32616, 32075, 32210, 28822, 30823, 29632, 29157, 31585, 24133, 23245, 22516, 18513, 18330, 15450, 12685, 11451, 11280, 9116, 7975, 8263, 8203, 4641, 5232, 5724, 4347, 4319, 3045, 1099, 2035, 2411, 1727, 852, 1134, 966, 2838, 6033, 2319, 3294, 3587, 9076, 5194, 6725, 6032, 6444, 10293, 9507, 10881, 11036, 12789, 12813, 14893, 16465, 16336, 16854, 19249, 23126, 21461, 18657, 20474, 24871, 20046, 22832, 21681, 21978, 23053, 20569, 24801, 19045, 20092, 19470, 18446, 18851, 18210, 15078, 16309, 15055, 14427, 15074, 10776, 14319, 14183, 7984, 8344, 7071, 9675, 5985, 3679, 2321, 6757, 3291, 5003, 1401, 1724, 1857, 2605, 803, 2742, 2971, 2306, 3722, 3332, 4427, 5762, 5383, 7692, 8436, 13660, 8018, 9303, 10626, 16171, 14163, 17161, 19214, 21171, 17274, 20616, 18281, 21171, 18220, 19315, 22558, 21393, 22431, 20186, 24619, 21997, 23938, 20029, 20694, 20648, 21173, 20377, 19147, 18578, 16839, 15735, 15907, 18059, 12111, 12178, 11201, 10577, 11160, 8485, 7065, 7852, 5865, 4856, 3955, 6803, 3444, 1616, 717, 3105, 704, 1473, 1948, 4534, 5800, 1757, 1038, 2435, 4677, 8155, 6870, 4611, 5372, 6304, 7868, 10336, 9091 };
size_t valuesLength = sizeof(values) / sizeof(values[0]);

int getMeasure()
{
	size_t static index = 0;
	index++;
	return values[index - 1];
}

DoubleEMAFilter<int> doubleEMAFilter(0.025, 0.1);

void setup() {
	Serial.begin(115200);

	for (size_t iCount = 0; iCount < valuesLength; iCount++)
	{
		// Obtener medicion simulada
		int rawMeasure = getMeasure();

		// Calcular filtro
		doubleEMAFilter.AddValue(rawMeasure);

		// Mostrar resultados
		// Emplear Serial Plotter para visualización gráfica
		Serial.print(rawMeasure);
		Serial.print(",\t");
		Serial.print(doubleEMAFilter.GetBandPass());
		Serial.print(",\t");
		Serial.println(doubleEMAFilter.GetBandStop());
	}
}

void loop()
{
}
