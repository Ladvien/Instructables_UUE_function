int UUEncode(uint8_t * UUE_data_array, uint8_t * hex_data_array, int hex_data_array_size)
{
	// 1. Add char for characters per line.
	// 2. Load 3 bytes into an array.
	// 3. Encode array.
	// 4. Add padding.
	// 5. Replace ' ' with '''
	// 6. Return UUE data array (implicit) and size.
	uint8_t byte_to_encode[3];
	uint8_t uue_char[4];

	int UUEncoded_array_index = 0;
	int uue_length_char_index = 45;
	int padded_index = 0;
	int bytes_left = 0;

	// 1. Add char for characters per line.
	if(hex_data_array_size < 45)
	{
		 UUE_data_array[UUEncoded_array_index] = ((hex_data_array_size & 0x3f) + ' ');
	}
	else
	{
		UUE_data_array[UUEncoded_array_index] = 'M';
	}

	UUEncoded_array_index++;

	// Encode loop.
	for (int hex_data_array_index = 0; hex_data_array_index < hex_data_array_size; hex_data_array_index)
	{
		// 2. Load 3 bytes into an array.
		for (int i = 0; i < 3; ++i)
		{
			// Load bytes into array
			if (hex_data_array_index < hex_data_array_size)
			{
				byte_to_encode[i] = hex_data_array[hex_data_array_index];
				hex_data_array_index++;
			}
			else
			{
				// 4. Add padding.
				byte_to_encode[i] = 0;
				padded_index++;
			}
			uue_length_char_index--;
		}

		// 3. Encode array.
		uue_char[0] = ((byte_to_encode[0] >> 2) & 0x3f);
		uue_char[1] = (((byte_to_encode[0] << 4) | ((byte_to_encode[1] >> 4) & 0x0f)) & 0x3f);
		uue_char[2] = (((byte_to_encode[1] << 2) | ((byte_to_encode[2] >> 6) & 0x03)) & 0x3f);
		uue_char[3] = (byte_to_encode[2] & 0x3f);

		for (int i = 0; i < 4; i++)
		{
			// 5. Replace ' ' with '''
			if (uue_char[i] == 0x00)
			{
				UUE_data_array[UUEncoded_array_index] = 0x60;
			}
			else
			{
				UUE_data_array[UUEncoded_array_index] = (uue_char[i] + ' ');
			}

			UUEncoded_array_index++;
		}

		// Data bytes left.
		bytes_left = (hex_data_array_size - hex_data_array_index);

		if (uue_length_char_index == 0 && bytes_left > 0)
		{
			// NOTE: Could be simplified to include first char
			// and additional characters, using a positive index.
			// 1. Add char for characters per line.
			UUE_data_array[UUEncoded_array_index] = '\n';
			UUEncoded_array_index++;

			if(bytes_left < 45)
			{
				// Find how many characters are left.
				UUE_data_array[UUEncoded_array_index] = ((bytes_left & 0x3f) + ' ');
			}
			else
			{
				UUE_data_array[UUEncoded_array_index] = 'M';
			}	
			UUEncoded_array_index++;
			uue_length_char_index = 45;
		}

	} // End UUE loop	
	UUE_data_array[UUEncoded_array_index] = '\n';

	// 6. Return UUE data array (implicit) and size.
	return UUEncoded_array_index;
}

int check_sum(uint8_t * hex_data_array, int hex_data_array_size)
{
	int check_sum = 0;
	int char_index = 0;

	while(char_index < hex_data_array_size)
	{
		check_sum += hex_data_array[char_index];
		char_index++;
	}
	return check_sum;
}