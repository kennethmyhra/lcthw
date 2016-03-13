set -e

if [ -f ./dbt ]; then
	echo "Deleting database dbt"
	rm dbt
fi

echo ""
echo "Creating database 'dbt' with row size 100 and data size of 512"
./ex17-2 dbt c 100 512

echo "Creating a data set"
./ex17-2 dbt s 0 "Kenneth Myhra" kennethmyhra@gmail.com
./ex17-2 dbt s 1 "Rungnapa Buasamui" bua_68@hotmail.com
./ex17-2 dbt s 2 "Monicha Christiansen" monicha.christiansen@me.com
./ex17-2 dbt s 3 "Geir Myhra" gmyhra@gmail.com
./ex17-2 dbt s 98 "Wibeche Myhra" wibeche@gmail.com
./ex17-2 dbt s 97 Mayken mayken@gmail.com
./ex17-2 dbt s 96 Jenny jenny@gmail.com
./ex17-2 dbt s 95 Eskil eskil@gmail.com
./ex17-2 dbt s 94 "Iben Elise" iben@gmail.com
./ex17-2 dbt s 93 Else elsem@gmail.com
./ex17-2 dbt s 92 "Tull og fanteri" fanteri@gmail.com

echo "Getting the values for a few rows"
./ex17-2 dbt g 0
./ex17-2 dbt g 1
./ex17-2 dbt g 92
./ex17-2 dbt g 97

echo "List contents"
./ex17-2 dbt l

echo "Execute search: 'Kenneth Myhra'"
./ex17-2 dbt f "Kenneth Myhra"
echo "Execute search: 'Monicha'"
./ex17-2 dbt f Monicha

echo "Delete row: 92"
./ex17-2 dbt d 92

echo "List contents after row 92 was deleted"
./ex17-2 dbt l
