program db;

var
    f1: file of integer;
    nomer: integer;
    f2: file of string[10];
    name: string[10];
    f3: file of string[10];
    lname: string[10];
    f4: file of char;
    gender: char;
    f5: file of integer;
    birth: integer;
    f6: file of char;
    discrypt: string;
    str: string;
    warning: string[50];
    _case: char;
    flag: boolean;

procedure help;
begin
    writeln('Доступные команды:');
    writeln('open - открытие/создание базы');
    writeln('print - выведение базы на экран');
    writeln('add - добавление новой записи в конец списка');
    writeln('redact - редактирование записи //comming soon');
    writeln('exit - выход');
    writeln('sort - сортировка //comming soon');
    writeln('delete - удаление записи //comming soon');
    writeln('достаточно писать только первую букву.');
end;

procedure open;
var
    i: integer;
    f: string;
begin
    flag := true;
    writeln('Введите название базы данных, в вслучае отсутствия будет создна пустая база данных');
    readln(f);
    assign(f1, f + '/' + 'f1');
    assign(f2, f + '/' + 'f2');
    assign(f3, f + '/' + 'f3');
    assign(f4, f + '/' + 'f4');
    assign(f5, f + '/' + 'f5');
    assign(f6, f + '/' + 'f6');
    if (FileExists( f + '/' + 'f1') = false) then 
        for i := 1 to 6 do 
        begin
            MKDIR(f);
            rewrite(f1);
            rewrite(f2);
            rewrite(f3);
            rewrite(f4);
            rewrite(f5);
            rewrite(f6);
        end;
end;

procedure add();
var
    i: integer;
begin
    //read---------
    if flag = false then  writeln(warning) else begin
        writeln('Введите новую запись вида: Фамилия(15) Имя(15) Пол(1) Дата(6) Описание');
        readln(str);
        name := '';
        lname := '';
        discrypt := '';
        for i := 1 to length(str) do
            if str[i] <> ' ' then 
                name := name + str[i] else break;
        for i := i + 1 to length(str) do
            if str[i] <> ' ' then 
                lname := lname + str[i] else break;
        gender := str[i + 1];
        birth := strtoint(copy(str, i + 3, 6));
        for i := i + 10 to length(str) do
            discrypt := discrypt + str[i];
        //-------------
        //write to file
        reset(f1);
        Seek(f1, filesize(f1));
        write(f1, filesize(f1));
        close(f1);
        reset(f2);
        Seek(f2, filesize(f2));
        write(f2, name);
        close(f2);
        reset(f3);
        Seek(f3, filesize(f3));
        write(f3, lname);
        close(f3);
        reset(f4);
        Seek(f4, filesize(f4));
        write(f4, gender);
        close(f4);
        reset(f5);
        Seek(f5, filesize(f5));
        write(f5, birth);
        close(f5);
        reset(f6);
        Seek(f6, filesize(f6));
        
        write(f6, chr(length(discrypt)));
        for i := 1 to length(discrypt) do
            write(f6, discrypt[i]);
        close(f6);
    end;
    //-------------
end;

procedure print();
var
    i, j, x, k: integer;
    c, dis: char;
begin
    if flag = false then writeln(warning) else begin
        x := 0;
        writeln('  Номер':9, 'Фамилия':13, 'Имя':12, 'Пол':8, 'Дата':6, 'Описание':14);
        reset(f1);
        x := 0;
        for i := 0 to filesize(f1) - 1 do 
        begin
            write(i + 1, ') ');
            
            reset(f1);
            Seek(f1, i);
            Read(f1, nomer);
            close(f1);
            reset(f2);
            Seek(f2, i);
            read(f2, name);
            close(f2);
            reset(f3);
            Seek(f3, i);
            read(f3, lname);
            close(f3);
            reset(f4);
            Seek(f4, i);
            read(f4, gender);
            close(f4);
            reset(f5);
            Seek(f5, i);
            read(f5, birth);
            close(f5);
            reset(f6);
            discrypt := '';
            Seek(f6, x);
            read(f6, c);
            x := x + ord(c) + 1;
            for j := 1 to ord(c) do 
            begin
                read(f6, dis);
                discrypt := discrypt + dis;
            end;
            writeln(nomer:4, name:15, lname:15, gender:4, birth:8, '  ', discrypt);
        end;
    end;
end;
//
{procedure delet();
var
    i: integer;
begin
    writeln('Введите номер удаляемой записи');
    readln(nomer);
    reset(f1);
    reset(f2);
    reset(f3);
    reset(f4);
    reset(f5);
    reset(f6);
    Seek(f1, nomer);
    Seek(f2, nomer);
    Seek(f3, nomer);
    Seek(f4, nomer);
    Seek(f5, nomer);
    for i := nomer to filesize(f1) - 1 do 
    begin
    end;
end;}
procedure redact();
var i,j,x:integer;
c:char;
begin
    writeln('Введите номер редактируемой записи');
        //read
    x := 0;
    if flag = false then  writeln(warning) else begin
        writeln('Введите новые данные вида: Фамилия(15) Имя(15) Пол(1) Дата(6) Описание');
        readln(str);
        name := '';
        lname := '';
        discrypt := '';
        for i := 1 to length(str) do
            if str[i] <> ' ' then 
                name := name + str[i] else break;
        for i := i + 1 to length(str) do
            if str[i] <> ' ' then 
                lname := lname + str[i] else break;
        gender := str[i + 1];
        birth := strtoint(copy(str, i + 3, 6));
        for i := i + 10 to length(str) do
            discrypt := discrypt + str[i];
            //write to file
        reset(f2);
        Seek(f2, nomer);
        write(f2, name);
        close(f2);
        reset(f3);
        Seek(f3, nomer);
        write(f3, lname);
        close(f3);
        reset(f4);
        Seek(f4, nomer);
        write(f4, gender);
        close(f4);
        reset(f5);
        Seek(f5, nomer);
        write(f5, birth);
        close(f5);
        {    //f6
        reset(f6);
        for i := 0 to nomer do 
        begin
            Seek(f6, x);
            read(f6, c);
            x := x + ord(c) + 1;
        end;
        for j := 1 to ord(c) do 
        begin
            write(f6, discryp[j]);
        end;
        close(f6);
        }
    end;
end;

begin
    flag := false;
    writeln('Для вывода списка доступных команд введите help');
    while true Do
    
    begin
        warning := 'Ещё не один файл не был открыт';
      readln(_case);
      case (_case) Of 
        'o': open();
        'a': add();
            'p': print();
            'r': redact();
    //'d': delet();
            //  's': sort();
        'b': break;
       // 'test': test();
            'h': help();
      End;
    end;
    print();
end.
