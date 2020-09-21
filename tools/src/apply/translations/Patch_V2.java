package apply.translations;

import static apply.translations.TextrumentLocalePatchs.*;

public class Patch_V2 implements ActionApplier {
	@Override
	public void pushActions() {
			Action action;
			
			//二进制模式 ( Binary Mode )
			action = pushActionByPathFieldedIDForNameField("{sq:'Mode Binary', ar:'الوضع الثنائي', am:'ሁለትዮሽ ሁነታ', az:'Binary Mode', ga:'dénártha Mód', et:'binaarsed', eu:'Binary Mode', be:'двайковы рэжым', bg:'Binary Mode', is:'Binary Mode', pl:'Tryb binarny', bs:'Binary Mode', fa:'حالت دودویی', af:'binêre modus', da:'binær tilstand', de:'Binary-Modus', ru:'Двоичный режим', fr:'mode binaire', tl:'binary Mode', fi:'binäärimuodossa', fy:'Binary Mode', km:'របៀបគោលពីរ', ka:'ორობითი რეჟიმი', gu:'બાઈનરી સ્થિતિ', kk:'екілік режимі', ht:'binè Mode', ko:'바이너리 모드', ha:'binary Mode', nl:'Binary Mode', ky:'Экилик режим', gl:'modo binario', ca:'Mode binari', cs:'Režim binární', kn:'ಬೈನರಿ ಮೋಡ್', co:'Moda binariu', hr:'binarnom modu', ku:'Mode binary', la:'Modus binarii', lv:'Binary Mode', lo:'Mode Binary', lt:'dvejetainiai režimas', lb:'Duebelstäresystem Régime', ro:'Modul binar', mg:'mimari-droa Mode', mt:'binarju Modalità', mr:'बायनरी मोड', ml:'ബൈനറി മോഡ്', ms:'Mod binari', mk:'бинарен режим', mi:'Aratau-rua', mn:'Хоёрт хэлбэрийн файлыг нь горим', bn:'বাইনারি মোড', my:'binary Mode ကို', hmn:'binary hom', xh:'Mode Binary', zu:'Binary Mode', ne:'बाइनरी मोड', no:'Binary Mode', pa:'ਬਾਈਨਰੀ ਮੋਡ', pt:'Modo binário', ps:'دويال اکر', ny:'Mafilimu angaphunzitse bayinare', ja:'バイナリモード', sv:'binärt läge', sm:'Faiga Binary', sr:'binarni режим', st:'Mode Binary', si:'ද්විමය ප්රකාරය', eo:'duumaj Mode', sk:'režim binárne', sl:'binarni način', sw:'Mode binary', gd:'Binary Mode', ceb:'duha Mode', so:'binary Mode', tg:'Усули дуӣ', te:'బైనరీ మోడ్', ta:'இரும முறை', th:'โหมดไบนารี', tr:'İkili Mod', cy:'Binary Modd', ur:'ثنائی موڈ', uk:'двійковий режим', uz:'ikkilik uslubi', es:'Modo binario', iw:'מצב בינארי', el:'Δυαδική Λειτουργία', haw:'aeaie? aaeei', sd:'انگن موڊ', hu:'bináris módban', sn:'Binary Mode', hy:'Երկուական Տարբերակ', ig:'ọnụọgụ abụọ mode', it:'Modalità Binary', yi:'ביינערי מאָדע', hi:'द्विआधारी मोड', su:'binér Mode', id:'biner Modus', jw:'binar Mode', en:'Binary Mode', yo:'alakomeji mode', vi:'binary Chế độ', zh_TW:'二進制模式', zh_CN:'二进制模式(&B)'}", INSERT, "id", 45014, "Native-Langue", "Menu", "Main", "Commands");
			action.tagName="Item";
			
			//自定义工具栏 ( Customize Toolbar )
			action = pushActionByPathFieldedIDForNameField("{sq:'Customize Toolbar', ar:'تخصيص شريط الأدوات', am:'አብጅ አሞሌ', az:'Customize Toolbar', ga:'Saincheap Toolbar', et:'Kohanda tööriistariba', eu:'Pertsonalizatu Toolbar', be:'Настройка панэлі інструментаў', bg:'Customize Toolbar', is:'Aðlaga Toolbar', pl:'Dostosuj pasek narzędzi', bs:'Prilagodi Toolbar', fa:'سفارشی کردن نوار ابزار', af:'Toolbar', da:'Tilpas værktøjslinje', de:'Symbolleiste anpassen', ru:'Настройка панели инструментов', fr:'Personnaliser la barre d&#39;outils', tl:'customize Toolbar', fi:'Muokkaa työkalupalkkia', fy:'Oanpasse Toolbar', km:'របារឧបករណ៍តាមបំណង', ka:'Customize Toolbar', gu:'કસ્ટમાઇઝ ટૂલબાર', kk:'Құралдар тақтасын теңшеу', ht:'Customize Toolbar', ko:'도구 모음 사용자 정의', ha:'siffanta Toolbar', nl:'Pas knoppenbalk', ky:'Өзгөчөлөштүрүү алуу куралдар панели', gl:'Personalizar barra de ferramentas', ca:'Personalitzar barra d&#39;eines', cs:'Přizpůsob', kn:'ಕಸ್ಟಮೈಸ್ ಟೂಲ್ಬಾರ್', co:'persunalizà Toolbar', hr:'Prilagodba alatne trake', ku:'Customize Toolbar', la:'Mos Toolbar', lv:'pielāgot rīkjoslu', lo:'ແຕ່ງແຖບເຄື່ອງມື', lt:'Pritaikyti įrankių juostą', lb:'asëtzen Toolbar', ro:'Personalizați Bara de instrumente', mg:'Ataovy miavaka Toolbar', mt:'Customize Toolbar', mr:'सानुकूलित टूलबार', ml:'ഇഷ്ടാനുസൃതമാക്കുക ഉപകരണബാർ', ms:'Customize Toolbar', mk:'прилагодите лентата со алатки', mi:'Paeutauta Whakarite', mn:'Customize Toolbar', bn:'কাস্টমাইজ টুলবার', my:'Customize Toolbar ကို', hmn:'Customize Toolbar', xh:'yenza Ibar yesixhobo', zu:'Yenza ibha yamathuluzi e', ne:'अनुकूलित उपकरणपट्टी', no:'Tilpass verktøylinje', pa:'ਪਸੰਦੀ ਟੂਲਬਾਰ', pt:'Personalizar Barra de Ferramentas', ps:'دتنظيمولو ټولبار', ny:'Sinthani Toolbar', ja:'ツールバーのカスタマイズ', sv:'Anpassa verktygsfält', sm:'Customize Toolbar', sr:'Подешавање траке алата', st:'tlwaelanya Toolbar', si:'සකසුරුවම් මෙවලම් තීරුව', eo:'Agordu Ilobreto', sk:'Prispôsob', sl:'Prilagajanje orodne vrstice', sw:'kukufaa Toolbar', gd:'Gnàthaich Toolbar', ceb:'ipahiangay ang Bara sa Himan', so:'Customize Toolbar', tg:'дохили Toolbar', te:'అనుకూలపరచండి ఉపకరణపట్టీ', ta:'தனிப்பயனாக்கு கருவிப்பட்டை', th:'ปรับแต่งแถบเครื่องมือ', tr:'Araç çubuğunu özelleştir', cy:'Customize Toolbar', ur:'مرضی کے مطابق بنائیں ٹول بار', uk:'Налаштування панелі інструментів', uz:'Sozlash paneli', es:'Personalizar la barra de herramientas', iw:'סרגל הכלים התאמה אישית', el:'Προσαρμογή γραμμής εργαλείων', haw:'i hoʻopilikino &#39;ke hoʻoili&#39;', sd:'سهيڙڻ ٽول بار', hu:'Eszköztár testreszabása', sn:'Customize Toolbar', hy:'Անհատականացնել Գործիքադարակի', ig:'hazie ngwaọrụ', it:'Personalizza barra degli strumenti', yi:'קאַסטאַמייז מכשיר', hi:'कस्टमाइज़ टूलबार', su:'ngaropea Toolbar', id:'Customize Toolbar', jw:'Ngatur Toolbar', en:'Customize Toolbar', yo:'Ṣe Toolbar', vi:'Tùy chỉnh thanh công cụ', zh_TW:'自定義工具欄', zh_CN:'自定义工具栏'}", INSERT, "id", 48105, "Native-Langue", "Menu", "Main", "Commands");
			action.tagName="Item";
			
			//工具栏换行 ( Wrap Toolbar )
			action = pushActionByPathFieldedIDForNameField("{sq:'Wrap Toolbar', ar:'شريط أدوات التفاف', am:'ጠቅልል አሞሌ', az:'Wrap Toolbar', ga:'wrap Toolbar', et:'wrap tööriistariba', eu:'Wrap Toolbar', be:'Wrap панэль інструментаў', bg:'Wrap Toolbar', is:'hula Toolbar', pl:'Wrap Toolbar', bs:'wrap Toolbar', fa:'قرار دادن نوار ابزار', af:'wrap Toolbar', da:'Wrap Toolbar', de:'Wrap Toolbar', ru:'Wrap панель инструментов', fr:'Barre d&#39;outils wrap', tl:'I-wrap ang Toolbar', fi:'wrap Toolbar', fy:'wrap Toolbar', km:'របារឧបករណ៍រុំ', ka:'Wrap Toolbar', gu:'વીંટો ટૂલબાર', kk:'Wrap құралдар тақтасы', ht:'vlope Toolbar', ko:'랩 도구 모음', ha:'kunsa Toolbar', nl:'wrap Toolbar', ky:'жыйынтыктар алуу куралдар панели', gl:'envoltura Toolbar', ca:'Barra d&#39;eines d&#39;embolcall', cs:'Wrap Toolbar', kn:'ಸುತ್ತು ಟೂಲ್ಬಾರ್', co:'kidnapping Toolbar', hr:'Zamotajte Toolbar', ku:'wrap Amûran', la:'wrap Toolbar', lv:'wrap rīkjosla', lo:'ຫໍ່ແຖບເຄື່ອງມື', lt:'įvyniojimas \"įrankių juosta', lb:'geschenkt Toolbar', ro:'Bara de instrumente Wrap', mg:'Wrap Toolbar', mt:'Kebbeb Toolbar', mr:'ओघ टूलबार', ml:'പൊതിയൽ ഉപകരണബാർ', ms:'balut Toolbar', mk:'заврши Toolbar-от', mi:'Paeutauta tākai', mn:'Боодолтой Toolbar', bn:'মোড়ানো টুলবার', my:'ထုပ်ပိုးပြီး Toolbar ကို', hmn:'muab Toolbar', xh:'wrap Ibar yesixhobo', zu:'Goqa ibha yamathuluzi e', ne:'र्याप उपकरणपट्टी', no:'wrap verktøylinjen', pa:'ਸਮੇਟੋ ਟੂਲਬਾਰ', pt:'envoltório Toolbar', ps:'غبرګونه ټولبار', ny:'Manga Toolbar', ja:'ラップツールバー', sv:'wrap Toolbar', sm:'afifi Toolbar', sr:'врап Тоолбар-', st:'phuthella Toolbar', si:'එතුම මෙවලම් තීරුව', eo:'wrap Ilobreto', sk:'Wrap Toolbar', sl:'Wrap Orodna vrstica', sw:'wrap Toolbar', gd:'Wrap Toolbar', ceb:'wrap Bara sa Himan', so:'duubo Toolbar', tg:'табии Toolbar', te:'సర్దుబాటు ఉపకరణపట్టీ', ta:'மடக்கு கருவிப்பட்டை', th:'ตัดแถบเครื่องมือ', tr:'Wrap Çubuğu', cy:'wrap Bar Offer', ur:'ملفوف ٹول بار', uk:'Wrap панель інструментів', uz:'Wrap paneli', es:'Barra de herramientas de envoltura', iw:'סרגל כלי גלישה', el:'Τυλίξτε Toolbar', haw:'wahī ke hoʻoili &#39;', sd:'لپيٽڻ ٽول بار', hu:'Wrap eszköztár', sn:'putira Toolbar', hy:'փաթեթավորեք Գործիքադարակի', ig:'Kechie ngwaọrụ', it:'Wrap Toolbar', yi:'ייַנוויקלען מכשיר', hi:'लपेटें टूलबार', su:'bungkus Toolbar', id:'wrap Toolbar', jw:'wrap Toolbar', en:'Wrap Toolbar', yo:'ipari Toolbar', vi:'bọc Toolbar', zh_TW:'裹工具欄', zh_CN:'工具栏换行(&W)'}", INSERT, "id", 48106, "Native-Langue", "Menu", "Main", "Commands");
			action.tagName="Item";
			
			//多行标签页 ( Multi-line Tabs )
			action = pushActionByPathFieldedIDForNameField("{sq:'Tabs Multi-line', ar:'علامات متعددة خط', am:'ባለ ብዙ መስመር ትሮች', az:'Multi-line Tabs', ga:'Cluaisíní il-líne', et:'Mitmeliiniline lehtedest', eu:'Lerro anitzeko Tabs', be:'шматканальны Укладкі', bg:'Multi-линия Tabs', is:'Multi-lína flipa', pl:'Multi-Tabs linia', bs:'Multi-line Kartice', fa:'زبانهها چند خط', af:'Multi-lyn Tabs', da:'Multi-line Tabs', de:'Multi-line Tabs', ru:'Многоканальный Вкладки', fr:'Les onglets multilignes', tl:'Multi-line Tabs', fi:'Multi-line välilehdet', fy:'Multi-line Tabs', km:'ផ្ទាំងពហុបន្ទាត់', ka:'Multi-line Tabs', gu:'મલ્ટી-લાઈન ટૅબ્સ', kk:'Мульти желісі қойындылар', ht:'Multi-liy Tabs', ko:'멀티 라인 탭', ha:'Multi-line Shafuka', nl:'Multi-line Tabs', ky:'Көп-сап Аккорды', gl:'Tabs Múltiples liña', ca:'aquí multilínia', cs:'Multi-linka Tabs', kn:'ಬಹು-ಸಾಲು ಟ್ಯಾಬ್ಗಳು', co:'Tabulature Multi-ligna', hr:'Višelinijski kartice', ku:'Tabs Multi-line', la:'Multi-linea Lyrics', lv:'Multi-line Tabs', lo:'Tabs ຫຼາຍເສັ້ນ', lt:'Kelių linijų Skirtukai', lb:'Multi-Linn Tabs', ro:'Tab-uri cu mai multe linii', mg:'Multi-dalana, Kihebaheba', mt:'Tabs multi-line', mr:'मल्टी-लाइन टॅब', ml:'മൾട്ടി-ലൈൻ ടാബുകൾ', ms:'Tabs berbilang talian', mk:'Мулти-линија Батерии', mi:'Ripa raina Maha-', mn:'Олон шугам Цонхнууд нь', bn:'মাল্টি লাইন ট্যাব', my:'multi-line ကိုတဲပ်များ', hmn:'Multi-line Tabs', xh:'Multi-line iiThebhu', zu:'Imigqa eminingi Amathebhu', ne:'बहु-लाइन ट्याबहरू', no:'Multi-line Tabs', pa:'ਮਲਟੀ-ਲਾਈਨ ਟੈਬਸ', pt:'Tabs Multi-line', ps:'څو کرښې په ټوپونو', ny:'Mipikisano mzere Masamba', ja:'マルチラインタブ', sv:'Flera linjer Tabs', sm:'Tabs eseese laina', sr:'Мулти-лине Картице', st:'Multi-line ya ditab', si:'බහු-රේඛාව ටැබ්', eo:'Plurliniaj Langetoj', sk:'Multi-linka Tabs', sl:'Zavihki večvrstična', sw:'Yenye mistari mingi Tabs', gd:'Ioma-loidhne Tabs', ceb:'Multi-linya Tabs', so:'ILAALI Multi-line', tg:'Ҷадвалбандҳо Мулти-хати', te:'బహుళ లైన్ టాబ్లు', ta:'மல்டி வரி தாவல்கள்', th:'แท็บหลายสาย', tr:'Çok çizgili Sekmeler', cy:'Tabs Aml-lein', ur:'کثیر لائن ٹیبز', uk:'багатоканальний Виберіть', uz:'Multi-line tablar', es:'Aquí multilínea', iw:'טאבים רב-קוי', el:'Καρτέλες πολλαπλών γραμμών', haw:'Nunui-laina nā māhele', sd:'مليل-ليڪ Tabs', hu:'Multi-line lapok', sn:'Multi-mutsetse Tabs', hy:'Multi-line Tabs', ig:'Multi-akara Taabụ', it:'Schede multilinea', yi:'מולטי-שורה טאַבס', hi:'बहु-रेखा टैब्स', su:'Tab multi garis', id:'Tab Multi-baris', jw:'Tabs Multi-line', en:'Multi-line Tabs', yo:'Multi-ila taabu', vi:'Tabs nhiều đường', zh_TW:'多行標籤', zh_CN:'多行标签页(&M)'}", INSERT, "id", 48107, "Native-Langue", "Menu", "Main", "Commands");
			action.tagName="Item";
			
			//空 ( Empty )
			action = pushActionByPathFieldedIDForNameField("{sq:'bosh', ar:'فارغة', am:'ባዶ', az:'boş', ga:'Folamh', et:'Tühi', eu:'Empty', be:'пусты', bg:'празен', is:'tómur', pl:'Pusty', bs:'prazan', fa:'خالی', af:'leë', da:'Tom', de:'Leer', ru:'пустой', fr:'Vide', tl:'walang laman', fi:'Tyhjä', fy:'Leech', km:'ទទេ', ka:'Empty', gu:'ખાલી', kk:'Бос', ht:'Vide', ko:'빈', ha:'komai', nl:'Leeg', ky:'бош', gl:'branco', ca:'Buit', cs:'Prázdný', kn:'ಖಾಲಿ', co:'viotu', hr:'Prazan', ku:'Vala', la:'inanis', lv:'tukšs', lo:'ຫວ່າງເປົ່າ', lt:'Tuščia', lb:'eidel', ro:'Gol', mg:'Empty', mt:'vojta', mr:'रिक्त', ml:'ഒഴിഞ്ഞ', ms:'kosong', mk:'Испразни', mi:'kau', mn:'хоосон', bn:'খালি', my:'ဗလာ', hmn:'npliag', xh:'Empty', zu:'engenalutho', ne:'खाली', no:'Tømme', pa:'ਖਾਲੀ', pt:'Vazio', ps:'تش', ny:'kanthu', ja:'空の', sv:'Tömma', sm:'gaogao', sr:'празан', st:'Empty', si:'හිස්', eo:'malplena', sk:'prázdny', sl:'Prazno', sw:'Empty', gd:'Empty', ceb:'walay sulod', so:'faaruqa', tg:'холӣ', te:'ఖాళీ', ta:'காலியாக', th:'ว่างเปล่า', tr:'Boş', cy:'Gwag', ur:'خالی', uk:'порожній', uz:'bo&#39;sh', es:'Vacío', iw:'ריק', el:'Αδειάζω', haw:'Empty', sd:'خالي', hu:'Üres', sn:'Empty', hy:'դատարկ', ig:'efu', it:'Vuoto', yi:'ליידיק', hi:'खाली', su:'kosong', id:'Kosong', jw:'P', en:'Dummy', yo:'sofo', vi:'Trống', zh_TW:'空', zh_CN:'空'}", INSERT, "id", 0x7FFFFF9B, "Native-Langue", "Dialog", "ShortcutMapper", "MainCommandNames");
			action.tagName="Item";
			action.mkdirs=true;

			pushActionByPathFieldedIDForNameField("{zh_CN:'管理快捷键(&S)...'}", MODIFY, "id", 48009, "Native-Langue", "Menu", "Main", "Commands");
			
			pushActionByPathFieldedIDForNameField("{zh_CN:'打开所在文件夹(&O)...'}", MODIFY, "CMID", 19, "Native-Langue", "Menu", "TabBar");
			
			pushActionByPathFieldedIDForNameField("{zh_CN:'重新载入(&R)...'}", MODIFY, "CMID", 16, "Native-Langue", "Menu", "TabBar");
			
			pushActionByPathFieldedIDForNameField("{zh_CN:'另存为其他(&S)...'}", MODIFY, "CMID", 3, "Native-Langue", "Menu", "TabBar");
			
			pushActionByPathFieldedIDForNameField("{zh_CN:'复制完整路径到剪切板(&F)'}", MODIFY, "CMID", 7, "Native-Langue", "Menu", "TabBar");
			
			pushActionByPathFieldedIDForNameField("{zh_CN:'编辑弹出菜单...'}", MODIFY, "id", 48018, "Native-Langue", "Menu", "Main", "Commands");
	}
}
