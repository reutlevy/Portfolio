import sqlite3
import csv  # Use this to read the csv file


def create_connection(db_file):
    """ create a database connection to the SQLite database
        specified by the db_file

    Parameters
    ----------
    Connection
    """
    sqlite3.connect(db_file)

    pass


def update_employee_salaries(conn, increase):
    cursor = conn.cursor()
    cursor.execute("""SELECT * FROM Employee JOIN ConstructorEmployee ON Employee.EID = ConstructorEmployee.EID WHERE (date('now')-Employee.BirthDate>=50) """)
    result = cursor.fetchall()
    for row in result:
        salary = int(row[10])*((100 + increase)/100)
        EIDD = int(row[0])
        cursor.execute('UPDATE ConstructorEmployee SET SalaryPerDay = ? WHERE EID=?', (salary, EIDD))

    """

    Parameters
    ----------
    conn: Connection
    increase: float
    """
    pass


def get_employee_total_salary(conn):
    cursor = conn.cursor()
    cursor.execute("""SELECT SUM(SalaryPerDay) as total_salary FROM ConstructorEmployee""")
    total = cursor.fetchall()
    for row in total:
        tot = int(row[0])
        return tot
    """
    Parameters
    ----------
    conn: Connection

    Returns
    -------
    int
    """
    pass


def get_total_projects_budget(conn):

    cursor = conn.cursor()
    cursor.execute("""SELECT SUM(Budget) as total FROM Project""")
    total = cursor.fetchone()
    tot = int(total[0])
    return tot
    pass


def calculate_income_from_parking(conn, year):
    cur = conn.cursor()
    cur.execute("SELECT SUM(Cost) FROM CarParking WHERE StartTime>=?",
                (year,))
    sum = cur.fetchone()
    total = float(sum[0])
    return total
    """
    Parameters
    ----------
    conn: Connection
    year: str

    Returns
    -------
    float
    """
    pass


def get_most_profitable_parking_areas(conn):
    cur = conn.cursor()
    cur.execute("""SELECT ParkingArea.AID,SUM(CarParking.Cost) as sum FROM CarParking JOIN 
                ParkingArea ON ParkingArea.AID = CarParking.AID
                GROUP BY CarParking.AID
                ORDER BY sum DESC, CarParking.AID DESC""")
    result = cur.fetchall()
    return result

    """
    Parameters
    ----------
    conn: Connection

    Returns
    -------
    list[tuple]

    """
    pass


def get_number_of_distinct_cars_by_area(conn):
    cur = conn.cursor()
    cur.execute("""SELECT ParkingArea.AID,count(DISTINCT CarParking.CID) as sum FROM CarParking JOIN 
                ParkingArea ON ParkingArea.AID = CarParking.AID
                GROUP BY CarParking.AID
                ORDER BY sum DESC, CarParking.AID 
                """)
    result = cur.fetchall()
    return result

    """
    Parameters
    ----------
    conn: Connection

    Returns
    -------
    list[tuple]

    """
    pass


def add_employee(conn, eid, firstname, lastname, birthdate, street_name, number, door, city):
    cur = conn.cursor()
    cur.execute('INSERT INTO Employee VALUES (? , ?, ?, ?, ?, ?, ?, ?)',
                (eid, firstname, lastname, birthdate, door, number, street_name, city))
    pass


def load_neighborhoods(conn, csv_path):
    with open(csv_path) as cs:
        cursor = conn.cursor()
        result = cs.readlines()
        for row in result:
            cursor.execute('INSERT INTO Neighborhood(NID,Name) VALUES (?,?)', (row[0], row[2]))
    pass
