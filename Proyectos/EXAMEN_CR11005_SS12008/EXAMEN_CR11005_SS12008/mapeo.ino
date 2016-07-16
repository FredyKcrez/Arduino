/*
 * Funcion que permite mapear el valor leido del potenciometro de un rango de 0 - 1023 a 0 - 5v con un valor decimal.
 */

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
