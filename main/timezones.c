#include "timezones.h"
#include <string.h>

// List of standard timezones
TimezoneEntry timezone_map[] = {
    {"Africa/Abidjan", 0},
    {"Africa/Accra", 1},
    {"Africa/Addis_Ababa", 2},
    {"Africa/Algiers", 3},
    {"Africa/Asmara", 4},
    {"Africa/Asmera", 5},
    {"Africa/Bamako", 6},
    {"Africa/Bangui", 7},
    {"Africa/Banjul", 8},
    {"Africa/Bissau", 9},
    {"Africa/Blantyre", 10},
    {"Africa/Brazzaville", 11},
    {"Africa/Bujumbura", 12},
    {"Africa/Cairo", 13},
    {"Africa/Casablanca", 14},
    {"Africa/Ceuta", 15},
    {"Africa/Conakry", 16},
    {"Africa/Dakar", 17},
    {"Africa/Dar_es_Salaam", 18},
    {"Africa/Djibouti", 19},
    {"Africa/Douala", 20},
    {"Africa/El_Aaiun", 21},
    {"Africa/Freetown", 22},
    {"Africa/Gaborone", 23},
    {"Africa/Harare", 24},
    {"Africa/Johannesburg", 25},
    {"Africa/Juba", 26},
    {"Africa/Kampala", 27},
    {"Africa/Khartoum", 28},
    {"Africa/Kigali", 29},
    {"Africa/Kinshasa", 30},
    {"Africa/Lagos", 31},
    {"Africa/Libreville", 32},
    {"Africa/Lome", 33},
    {"Africa/Luanda", 34},
    {"Africa/Lubumbashi", 35},
    {"Africa/Lusaka", 36},
    {"Africa/Malabo", 37},
    {"Africa/Maputo", 38},
    {"Africa/Maseru", 39},
    {"Africa/Mbabane", 40},
    {"Africa/Mogadishu", 41},
    {"Africa/Monrovia", 42},
    {"Africa/Nairobi", 43},
    {"Africa/Ndjamena", 44},
    {"Africa/Niamey", 45},
    {"Africa/Nouakchott", 46},
    {"Africa/Ouagadougou", 47},
    {"Africa/Porto-Novo", 48},
    {"Africa/Sao_Tome", 49},
    {"Africa/Timbuktu", 50},
    {"Africa/Tripoli", 51},
    {"Africa/Tunis", 52},
    {"Africa/Windhoek", 53},
    {"America/Adak", 54},
    {"America/Anchorage", 55},
    {"America/Anguilla", 56},
    {"America/Antigua", 57},
    {"America/Araguaina", 58},
    {"America/Argentina/Buenos_Aires", 59},
    {"America/Argentina/Catamarca", 60},
    {"America/Argentina/ComodRivadavia", 61},
    {"America/Argentina/Cordoba", 62},
    {"America/Argentina/Jujuy", 63},
    {"America/Argentina/La_Rioja", 64},
    {"America/Argentina/Mendoza", 65},
    {"America/Argentina/Rio_Gallegos", 66},
    {"America/Argentina/Salta", 67},
    {"America/Argentina/San_Juan", 68},
    {"America/Argentina/San_Luis", 69},
    {"America/Argentina/Tucuman", 70},
    {"America/Argentina/Ushuaia", 71},
    {"America/Aruba", 72},
    {"America/Asuncion", 73},
    {"America/Atikokan", 74},
    {"America/Atka", 75},
    {"America/Bahia", 76},
    {"America/Bahia_Banderas", 77},
    {"America/Barbados", 78},
    {"America/Belem", 79},
    {"America/Belize", 80},
    {"America/Blanc-Sablon", 81},
    {"America/Boa_Vista", 82},
    {"America/Bogota", 83},
    {"America/Boise", 84},
    {"America/Buenos_Aires", 85},
    {"America/Cambridge_Bay", 86},
    {"America/Campo_Grande", 87},
    {"America/Cancun", 88},
    {"America/Caracas", 89},
    {"America/Catamarca", 90},
    {"America/Cayenne", 91},
    {"America/Cayman", 92},
    {"America/Chicago", 93},
    {"America/Chihuahua", 94},
    {"America/Ciudad_Juarez", 95},
    {"America/Coral_Harbour", 96},
    {"America/Cordoba", 97},
    {"America/Costa_Rica", 98},
    {"America/Creston", 99},
    {"America/Cuiaba", 100},
    {"America/Curacao", 101},
    {"America/Danmarkshavn", 102},
    {"America/Dawson", 103},
    {"America/Dawson_Creek", 104},
    {"America/Denver", 105},
    {"America/Detroit", 106},
    {"America/Dominica", 107},
    {"America/Edmonton", 108},
    {"America/Eirunepe", 109},
    {"America/El_Salvador", 110},
    {"America/Ensenada", 111},
    {"America/Fort_Nelson", 112},
    {"America/Fort_Wayne", 113},
    {"America/Fortaleza", 114},
    {"America/Glace_Bay", 115},
    {"America/Godthab", 116},
    {"America/Goose_Bay", 117},
    {"America/Grand_Turk", 118},
    {"America/Grenada", 119},
    {"America/Guadeloupe", 120},
    {"America/Guatemala", 121},
    {"America/Guayaquil", 122},
    {"America/Guyana", 123},
    {"America/Halifax", 124},
    {"America/Havana", 125},
    {"America/Hermosillo", 126},
    {"America/Indiana/Indianapolis", 127},
    {"America/Indiana/Knox", 128},
    {"America/Indiana/Marengo", 129},
    {"America/Indiana/Petersburg", 130},
    {"America/Indiana/Tell_City", 131},
    {"America/Indiana/Vevay", 132},
    {"America/Indiana/Vincennes", 133},
    {"America/Indiana/Winamac", 134},
    {"America/Indianapolis", 135},
    {"America/Inuvik", 136},
    {"America/Iqaluit", 137},
    {"America/Jamaica", 138},
    {"America/Jujuy", 139},
    {"America/Juneau", 140},
    {"America/Kentucky/Louisville", 141},
    {"America/Kentucky/Monticello", 142},
    {"America/Knox_IN", 143},
    {"America/Kralendijk", 144},
    {"America/La_Paz", 145},
    {"America/Lima", 146},
    {"America/Los_Angeles", 147},
    {"America/Louisville", 148},
    {"America/Lower_Princes", 149},
    {"America/Maceio", 150},
    {"America/Managua", 151},
    {"America/Manaus", 152},
    {"America/Marigot", 153},
    {"America/Martinique", 154},
    {"America/Matamoros", 155},
    {"America/Mazatlan", 156},
    {"America/Mendoza", 157},
    {"America/Menominee", 158},
    {"America/Merida", 159},
    {"America/Metlakatla", 160},
    {"America/Mexico_City", 161},
    {"America/Miquelon", 162},
    {"America/Moncton", 163},
    {"America/Monterrey", 164},
    {"America/Montevideo", 165},
    {"America/Montreal", 166},
    {"America/Montserrat", 167},
    {"America/Nassau", 168},
    {"America/New_York", 169},
    {"America/Nipigon", 170},
    {"America/Nome", 171},
    {"America/Noronha", 172},
    {"America/North_Dakota/Beulah", 173},
    {"America/North_Dakota/Center", 174},
    {"America/North_Dakota/New_Salem", 175},
    {"America/Nuuk", 176},
    {"America/Ojinaga", 177},
    {"America/Panama", 178},
    {"America/Pangnirtung", 179},
    {"America/Paramaribo", 180},
    {"America/Phoenix", 181},
    {"America/Port-au-Prince", 182},
    {"America/Port_of_Spain", 183},
    {"America/Porto_Acre", 184},
    {"America/Porto_Velho", 185},
    {"America/Puerto_Rico", 186},
    {"America/Punta_Arenas", 187},
    {"America/Rainy_River", 188},
    {"America/Rankin_Inlet", 189},
    {"America/Recife", 190},
    {"America/Regina", 191},
    {"America/Resolute", 192},
    {"America/Rio_Branco", 193},
    {"America/Rosario", 194},
    {"America/Santa_Isabel", 195},
    {"America/Santarem", 196},
    {"America/Santiago", 197},
    {"America/Santo_Domingo", 198},
    {"America/Sao_Paulo", 199},
    {"America/Scoresbysund", 200},
    {"America/Shiprock", 201},
    {"America/Sitka", 202},
    {"America/St_Barthelemy", 203},
    {"America/St_Johns", 204},
    {"America/St_Kitts", 205},
    {"America/St_Lucia", 206},
    {"America/St_Thomas", 207},
    {"America/St_Vincent", 208},
    {"America/Swift_Current", 209},
    {"America/Tegucigalpa", 210},
    {"America/Thule", 211},
    {"America/Thunder_Bay", 212},
    {"America/Tijuana", 213},
    {"America/Toronto", 214},
    {"America/Tortola", 215},
    {"America/Vancouver", 216},
    {"America/Virgin", 217},
    {"America/Whitehorse", 218},
    {"America/Winnipeg", 219},
    {"America/Yakutat", 220},
    {"America/Yellowknife", 221},
    {"Antarctica/Casey", 222},
    {"Antarctica/Davis", 223},
    {"Antarctica/DumontDUrville", 224},
    {"Antarctica/Macquarie", 225},
    {"Antarctica/Mawson", 226},
    {"Antarctica/McMurdo", 227},
    {"Antarctica/Palmer", 228},
    {"Antarctica/Rothera", 229},
    {"Antarctica/South_Pole", 230},
    {"Antarctica/Syowa", 231},
    {"Antarctica/Troll", 232},
    {"Antarctica/Vostok", 233},
    {"Arctic/Longyearbyen", 234},
    {"Asia/Aden", 235},
    {"Asia/Almaty", 236},
    {"Asia/Amman", 237},
    {"Asia/Anadyr", 238},
    {"Asia/Aqtau", 239},
    {"Asia/Aqtobe", 240},
    {"Asia/Ashgabat", 241},
    {"Asia/Ashkhabad", 242},
    {"Asia/Atyrau", 243},
    {"Asia/Baghdad", 244},
    {"Asia/Bahrain", 245},
    {"Asia/Baku", 246},
    {"Asia/Bangkok", 247},
    {"Asia/Barnaul", 248},
    {"Asia/Beirut", 249},
    {"Asia/Bishkek", 250},
    {"Asia/Brunei", 251},
    {"Asia/Calcutta", 252},
    {"Asia/Chita", 253},
    {"Asia/Choibalsan", 254},
    {"Asia/Chongqing", 255},
    {"Asia/Chungking", 256},
    {"Asia/Colombo", 257},
    {"Asia/Dacca", 258},
    {"Asia/Damascus", 259},
    {"Asia/Dhaka", 260},
    {"Asia/Dili", 261},
    {"Asia/Dubai", 262},
    {"Asia/Dushanbe", 263},
    {"Asia/Famagusta", 264},
    {"Asia/Gaza", 265},
    {"Asia/Harbin", 266},
    {"Asia/Hebron", 267},
    {"Asia/Ho_Chi_Minh", 268},
    {"Asia/Hong_Kong", 269},
    {"Asia/Hovd", 270},
    {"Asia/Irkutsk", 271},
    {"Asia/Istanbul", 272},
    {"Asia/Jakarta", 273},
    {"Asia/Jayapura", 274},
    {"Asia/Jerusalem", 275},
    {"Asia/Kabul", 276},
    {"Asia/Kamchatka", 277},
    {"Asia/Karachi", 278},
    {"Asia/Kashgar", 279},
    {"Asia/Kathmandu", 280},
    {"Asia/Katmandu", 281},
    {"Asia/Khandyga", 282},
    {"Asia/Kolkata", 283},
    {"Asia/Krasnoyarsk", 284},
    {"Asia/Kuala_Lumpur", 285},
    {"Asia/Kuching", 286},
    {"Asia/Kuwait", 287},
    {"Asia/Macao", 288},
    {"Asia/Macau", 289},
    {"Asia/Magadan", 290},
    {"Asia/Makassar", 291},
    {"Asia/Manila", 292},
    {"Asia/Muscat", 293},
    {"Asia/Nicosia", 294},
    {"Asia/Novokuznetsk", 295},
    {"Asia/Novosibirsk", 296},
    {"Asia/Omsk", 297},
    {"Asia/Oral", 298},
    {"Asia/Phnom_Penh", 299},
    {"Asia/Pontianak", 300},
    {"Asia/Pyongyang", 301},
    {"Asia/Qatar", 302},
    {"Asia/Qostanay", 303},
    {"Asia/Qyzylorda", 304},
    {"Asia/Rangoon", 305},
    {"Asia/Riyadh", 306},
    {"Asia/Saigon", 307},
    {"Asia/Sakhalin", 308},
    {"Asia/Samarkand", 309},
    {"Asia/Seoul", 310},
    {"Asia/Shanghai", 311},
    {"Asia/Singapore", 312},
    {"Asia/Srednekolymsk", 313},
    {"Asia/Taipei", 314},
    {"Asia/Tashkent", 315},
    {"Asia/Tbilisi", 316},
    {"Asia/Tehran", 317},
    {"Asia/Tel_Aviv", 318},
    {"Asia/Thimbu", 319},
    {"Asia/Thimphu", 320},
    {"Asia/Tokyo", 321},
    {"Asia/Tomsk", 322},
    {"Asia/Ujung_Pandang", 323},
    {"Asia/Ulaanbaatar", 324},
    {"Asia/Ulan_Bator", 325},
    {"Asia/Urumqi", 326},
    {"Asia/Ust-Nera", 327},
    {"Asia/Vientiane", 328},
    {"Asia/Vladivostok", 329},
    {"Asia/Yakutsk", 330},
    {"Asia/Yangon", 331},
    {"Asia/Yekaterinburg", 332},
    {"Asia/Yerevan", 333},
    {"Atlantic/Azores", 334},
    {"Atlantic/Bermuda", 335},
    {"Atlantic/Canary", 336},
    {"Atlantic/Cape_Verde", 337},
    {"Atlantic/Faeroe", 338},
    {"Atlantic/Faroe", 339},
    {"Atlantic/Jan_Mayen", 340},
    {"Atlantic/Madeira", 341},
    {"Atlantic/Reykjavik", 342},
    {"Atlantic/South_Georgia", 343},
    {"Atlantic/St_Helena", 344},
    {"Atlantic/Stanley", 345},
    {"Australia/ACT", 346},
    {"Australia/Adelaide", 347},
    {"Australia/Brisbane", 348},
    {"Australia/Broken_Hill", 349},
    {"Australia/Canberra", 350},
    {"Australia/Currie", 351},
    {"Australia/Darwin", 352},
    {"Australia/Eucla", 353},
    {"Australia/Hobart", 354},
    {"Australia/LHI", 355},
    {"Australia/Lindeman", 356},
    {"Australia/Lord_Howe", 357},
    {"Australia/Melbourne", 358},
    {"Australia/NSW", 359},
    {"Australia/North", 360},
    {"Australia/Perth", 361},
    {"Australia/Queensland", 362},
    {"Australia/South", 363},
    {"Australia/Sydney", 364},
    {"Australia/Tasmania", 365},
    {"Australia/Victoria", 366},
    {"Australia/West", 367},
    {"Australia/Yancowinna", 368},
    {"Brazil/Acre", 369},
    {"Brazil/DeNoronha", 370},
    {"Brazil/East", 371},
    {"Brazil/West", 372},
    {"CET", 373},
    {"CST6CDT", 374},
    {"Canada/Atlantic", 375},
    {"Canada/Central", 376},
    {"Canada/Eastern", 377},
    {"Canada/Mountain", 378},
    {"Canada/Newfoundland", 379},
    {"Canada/Pacific", 380},
    {"Canada/Saskatchewan", 381},
    {"Canada/Yukon", 382},
    {"Chile/Continental", 383},
    {"Chile/EasterIsland", 384},
    {"Cuba", 385},
    {"EET", 386},
    {"EST", 387},
    {"EST5EDT", 388},
    {"Egypt", 389},
    {"Eire", 390},
    {"Etc/GMT", 391},
    {"Etc/GMT+0", 392},
    {"Etc/GMT+1", 393},
    {"Etc/GMT+10", 394},
    {"Etc/GMT+11", 395},
    {"Etc/GMT+12", 396},
    {"Etc/GMT+2", 397},
    {"Etc/GMT+3", 398},
    {"Etc/GMT+4", 399},
    {"Etc/GMT+5", 400},
    {"Etc/GMT+6", 401},
    {"Etc/GMT+7", 402},
    {"Etc/GMT+8", 403},
    {"Etc/GMT+9", 404},
    {"Etc/GMT-0", 405},
    {"Etc/GMT-1", 406},
    {"Etc/GMT-10", 407},
    {"Etc/GMT-11", 408},
    {"Etc/GMT-12", 409},
    {"Etc/GMT-13", 410},
    {"Etc/GMT-14", 411},
    {"Etc/GMT-2", 412},
    {"Etc/GMT-3", 413},
    {"Etc/GMT-4", 414},
    {"Etc/GMT-5", 415},
    {"Etc/GMT-6", 416},
    {"Etc/GMT-7", 417},
    {"Etc/GMT-8", 418},
    {"Etc/GMT-9", 419},
    {"Etc/GMT0", 420},
    {"Etc/Greenwich", 421},
    {"Etc/UCT", 422},
    {"Etc/UTC", 423},
    {"Etc/Universal", 424},
    {"Etc/Zulu", 425},
    {"Europe/Amsterdam", 426},
    {"Europe/Andorra", 427},
    {"Europe/Astrakhan", 428},
    {"Europe/Athens", 429},
    {"Europe/Belfast", 430},
    {"Europe/Belgrade", 431},
    {"Europe/Berlin", 432},
    {"Europe/Bratislava", 433},
    {"Europe/Brussels", 434},
    {"Europe/Bucharest", 435},
    {"Europe/Budapest", 436},
    {"Europe/Busingen", 437},
    {"Europe/Chisinau", 438},
    {"Europe/Copenhagen", 439},
    {"Europe/Dublin", 440},
    {"Europe/Gibraltar", 441},
    {"Europe/Guernsey", 442},
    {"Europe/Helsinki", 443},
    {"Europe/Isle_of_Man", 444},
    {"Europe/Istanbul", 445},
    {"Europe/Jersey", 446},
    {"Europe/Kaliningrad", 447},
    {"Europe/Kiev", 448},
    {"Europe/Kirov", 449},
    {"Europe/Kyiv", 450},
    {"Europe/Lisbon", 451},
    {"Europe/Ljubljana", 452},
    {"Europe/London", 453},
    {"Europe/Luxembourg", 454},
    {"Europe/Madrid", 455},
    {"Europe/Malta", 456},
    {"Europe/Mariehamn", 457},
    {"Europe/Minsk", 458},
    {"Europe/Monaco", 459},
    {"Europe/Moscow", 460},
    {"Europe/Nicosia", 461},
    {"Europe/Oslo", 462},
    {"Europe/Paris", 463},
    {"Europe/Podgorica", 464},
    {"Europe/Prague", 465},
    {"Europe/Riga", 466},
    {"Europe/Rome", 467},
    {"Europe/Samara", 468},
    {"Europe/San_Marino", 469},
    {"Europe/Sarajevo", 470},
    {"Europe/Saratov", 471},
    {"Europe/Simferopol", 472},
    {"Europe/Skopje", 473},
    {"Europe/Sofia", 474},
    {"Europe/Stockholm", 475},
    {"Europe/Tallinn", 476},
    {"Europe/Tirane", 477},
    {"Europe/Tiraspol", 478},
    {"Europe/Ulyanovsk", 479},
    {"Europe/Uzhgorod", 480},
    {"Europe/Vaduz", 481},
    {"Europe/Vatican", 482},
    {"Europe/Vienna", 483},
    {"Europe/Vilnius", 484},
    {"Europe/Volgograd", 485},
    {"Europe/Warsaw", 486},
    {"Europe/Zagreb", 487},
    {"Europe/Zaporozhye", 488},
    {"Europe/Zurich", 489},
    {"GB", 490},
    {"GB-Eire", 491},
    {"GMT", 492},
    {"GMT+0", 493},
    {"GMT-0", 494},
    {"GMT0", 495},
    {"Greenwich", 496},
    {"HST", 497},
    {"Hongkong", 498},
    {"Iceland", 499},
    {"Indian/Antananarivo", 500},
    {"Indian/Chagos", 501},
    {"Indian/Christmas", 502},
    {"Indian/Cocos", 503},
    {"Indian/Comoro", 504},
    {"Indian/Kerguelen", 505},
    {"Indian/Mahe", 506},
    {"Indian/Maldives", 507},
    {"Indian/Mauritius", 508},
    {"Indian/Mayotte", 509},
    {"Indian/Reunion", 510},
    {"Iran", 511},
    {"Israel", 512},
    {"Jamaica", 513},
    {"Japan", 514},
    {"Kwajalein", 515},
    {"Libya", 516},
    {"MET", 517},
    {"MST", 518},
    {"MST7MDT", 519},
    {"Mexico/BajaNorte", 520},
    {"Mexico/BajaSur", 521},
    {"Mexico/General", 522},
    {"NZ", 523},
    {"NZ-CHAT", 524},
    {"Navajo", 525},
    {"PRC", 526},
    {"PST8PDT", 527},
    {"Pacific/Apia", 528},
    {"Pacific/Auckland", 529},
    {"Pacific/Bougainville", 530},
    {"Pacific/Chatham", 531},
    {"Pacific/Chuuk", 532},
    {"Pacific/Easter", 533},
    {"Pacific/Efate", 534},
    {"Pacific/Enderbury", 535},
    {"Pacific/Fakaofo", 536},
    {"Pacific/Fiji", 537},
    {"Pacific/Funafuti", 538},
    {"Pacific/Galapagos", 539},
    {"Pacific/Gambier", 540},
    {"Pacific/Guadalcanal", 541},
    {"Pacific/Guam", 542},
    {"Pacific/Honolulu", 543},
    {"Pacific/Johnston", 544},
    {"Pacific/Kanton", 545},
    {"Pacific/Kiritimati", 546},
    {"Pacific/Kosrae", 547},
    {"Pacific/Kwajalein", 548},
    {"Pacific/Majuro", 549},
    {"Pacific/Marquesas", 550},
    {"Pacific/Midway", 551},
    {"Pacific/Nauru", 552},
    {"Pacific/Niue", 553},
    {"Pacific/Norfolk", 554},
    {"Pacific/Noumea", 555},
    {"Pacific/Pago_Pago", 556},
    {"Pacific/Palau", 557},
    {"Pacific/Pitcairn", 558},
    {"Pacific/Pohnpei", 559},
    {"Pacific/Ponape", 560},
    {"Pacific/Port_Moresby", 561},
    {"Pacific/Rarotonga", 562},
    {"Pacific/Saipan", 563},
    {"Pacific/Samoa", 564},
    {"Pacific/Tahiti", 565},
    {"Pacific/Tarawa", 566},
    {"Pacific/Tongatapu", 567},
    {"Pacific/Truk", 568},
    {"Pacific/Wake", 569},
    {"Pacific/Wallis", 570},
    {"Pacific/Yap", 571},
    {"Poland", 572},
    {"Portugal", 573},
    {"ROC", 574},
    {"ROK", 575},
    {"Singapore", 576},
    {"Turkey", 577},
    {"UCT", 578},
    {"US/Alaska", 579},
    {"US/Aleutian", 580},
    {"US/Arizona", 581},
    {"US/Central", 582},
    {"US/East-Indiana", 583},
    {"US/Eastern", 584},
    {"US/Hawaii", 585},
    {"US/Indiana-Starke", 586},
    {"US/Michigan", 587},
    {"US/Mountain", 588},
    {"US/Pacific", 589},
    {"US/Samoa", 590},
    {"UTC", 591},
    {"Universal", 592},
    {"W-SU", 593},
    {"WET", 594},
    {"Zulu", 595}
};

// Total number of timezones
static const int timezone_count = sizeof(timezone_map) / sizeof(TimezoneEntry);

// Function to get the index of a timezone
int get_timezone_index(const char *timezone_id) {
    for (int i = 0; i < timezone_count; i++) {
        if (strcmp(timezone_map[i].timezone_id, timezone_id) == 0) {
            return timezone_map[i].index;
        }
    }
    return -1; // Return -1 if not found
}
