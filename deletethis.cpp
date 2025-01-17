//Записывает в файл строки
//void file_add_str(string& str, string& fileName) {
//	ofstream fout(fileName);
//	if (!fout.is_open())
//	{
//		const string _outerMsg = "Файл \" " + fileName + " \" не создан";
//		throw _outerMsg;
//	}
//	fout << str;
//	fout.close();
//}
//
////Читает файл и возвращает содержимое
//string file_read(string fileName) {
//	ifstream fin(fileName);
//	string _user_txt = "", _str;
//	if (!fin.is_open())
//	{
//		const string _outerMsg = "Файл \" " + fileName + " \" не найден";
//		throw _outerMsg;
//	}
//	while (getline(fin, _str))
//	{
//		_user_txt += _str + '\n';
//	}
//	//Закрытие файла для чтения
//	fin.close();
//	return _user_txt;
//}

//try {
	//	if (argc < 2) {//Вывод справки если вызвано без параметров
	//		throw int{ 1 };
	//	}
	//	
	//		

	//	if (_stricmp(argv[1], "/key") == 0) {
	//		isEncoder = true;
	//	}
	//	else if (_stricmp(argv[1], "/dec") == 0) {
	//		isEncoder = false;
	//	}
	//	else {
	//		throw int{ 1 };
	//	}
	//	if (argc >=4 ) {
	//		 key = argv[2]

	//		if (_stricmp(argv[3], "/f") == 0) {
	//			if (argc == 6) {
	//				isFileOut = true,
	//					isConIn = false;
	//				fromFile = argv[4];
	//				toFile = argv[5];
	//			}
	//			else {
	//				throw int{ 1 };
	//			}
	//		}
	//		else if (_stricmp(argv[3], "/t") == 0) {
	//			if (argc == 5) {
	//				isFileOut = true,
	//					isConIn = true;
	//				toFile = argv[4];
	//			}
	//			else {
	//				throw int{ 1 };
	//			}
	//		}
	//	}
	//	else {
	//		throw int{ 1 };
	//	}
	//	cout << isEncoder << '/t' << isFileOut << '/t' << isConIn << '/t';


	//}
	//catch (const string msg) {
	//	cout << endl << "const string msg: " << msg << endl;
	//}
	//catch (const char* msg) {
	//	cout << endl <<"char* msg: " << msg << endl;
	//}
	//catch (int erCode) {
	//	switch (erCode)
	//	{
	//	case 1:
	//		cout << file_read("./readme.txt");
	//		break;
	//	default:
	//		break;
	//	}
	//	return 0;
	//}
	//return 0;

//while (true) {
//	//Ввод исходных данных
//	cout << endl << "Введите тип задачи" << endl
//		<< "1 - Ввести строку с клавиатуры и обработать" << endl
//		<< "2 - для выхода" << endl << endl;
//	cin >> var;
//	switch (var) {
//		//Ввести строку с клавиатуры и обработать список
//	case 1: {
//		cout << endl << "Введите строку и нажмите <enter>" << endl;
//		cin.ignore();
//		getline(cin, userStr);
//		if (userStr == "") { throw "Исходная строкка пуста: Требуется хотя бы один символ"; }
//		cout << endl << "=== Вывод исходной строки ===" << endl
//			<< userStr << endl;

//		//Обработка строки
//		res = process_str(userStr);
//		//res = bar_process_str(userStr);

//		//Вывод обработанной строки
//		cout << endl << "=== Вывод обработанной строки ===" << endl << res << endl;

//	}break;
//	default:
//		return 0;
//	}
//}
/*
	bool numberLines = false;    // Default is no line numbers.

	// If /n is passed to the .exe, display numbered listing
	// of environment variables.
	if ((argc > 2) && _stricmp(argv[1], "/n") == 0)
		numberLines = true;

	// Walk through list of strings until a NULL is encountered.
	for (int i = 0; envp[i] != NULL; ++i)
	{
		if (numberLines)
			cout << i << ": "; // Prefix with numbers if /n specified
		cout << envp[i] << "\n";
	}
*/