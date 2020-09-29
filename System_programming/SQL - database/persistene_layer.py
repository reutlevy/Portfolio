import atexit
import sqlite3
import Dao
import Dto


# Repository
class Repository(object):

    def __init__(self):
        self._conn = sqlite3.connect('moncafe.db')
        self.employees = Dao._Employees(self._conn)
        self.suppliers = Dao._Suppliers(self._conn)
        self.products = Dao._Products(self._conn)
        self.coffee_stands = Dao._Coffee_stands(self._conn)
        self.activities = Dao._Activities(self._conn)

    def _close(self):
        self._conn.commit()
        self._conn.close()

    def create_tables(self):
        self._conn.executescript("""
        CREATE TABLE Employees (
            id              INTEGER         PRIMARY KEY,
            name            TEXT            NOT NULL,
            salary          REAL            NOT NULL,
            coffee_stand    INTEGER         NOT NULL,
            FOREIGN KEY(coffee_stand)       REFERENCES Coffee_stands(id)
        );

        CREATE TABLE Suppliers (
            id                   INTEGER         PRIMARY KEY,
            name                 TEXT            NOT NULL,
            contact_information  TEXT            
        );

        CREATE TABLE Products (
            id              INTEGER         PRIMARY KEY,
            description     TEXT            ,
            price           REAL            NOT NULL,
            quantity        INTEGER         NOT NULL 
        );

        CREATE TABLE Coffee_stands (
            id                      INTEGER         PRIMARY KEY,
            location                TEXT            NOT NULL,
            number_of_employees     INTEGER        
        );

        CREATE TABLE Activities (
            product_id      INTEGER        NOT NULL,
            quantity        INTEGER        NOT NULL,
            activator_id    INTEGER        NOT NULL,
            date            DATE           NOT NULL,
            FOREIGN KEY(product_id)     REFERENCES Products(id)

        );
    """)

    def printactivities(self):
        sql = """SELECT Activities.date, description, Activities.quantity , Employees.name EmployeeName, 
        Suppliers.name as SupplierName FROM ( ((Activities inner join Products on Products.id=Activities.product_id) 
        LEFT join Employees on Activities.activator_id=Employees.id) LEFT join Suppliers on Suppliers.id= 
        Activities.activator_id)order by Activities.date """
        conn = sqlite3.connect('moncafe.db')
        c = conn.cursor()
        c.execute(sql)
        myresult = c.fetchall()

        print("Activity_Report")
        for x in myresult:
            print(x)

    def printsales(self):
        sql = """SELECT name, salary, location, coalesce(sum(TotalSales),0) as TotalSales FROM ((SELECT DISTINCT 
        Employees.id, Employees.salary,(-1)*sales*price as TotalSales,Employees.name, Employees.coffee_stand, 
        location FROM ((((Employees LEFT JOIN (SELECT Activities.activator_id ,Activities.product_id, 
        Activities.quantity as sales FROM Activities) Activities ON Activities.activator_id=Employees.id)LEFT JOIN (
        SELECT Coffee_stands.location as location, Coffee_stands.id FROM Coffee_stands) Coffee_stands ON 
        Employees.coffee_stand=Coffee_stands.id) LEFT JOIN (SELECT Products.price as price, Products.id FROM 
        Products) Products ON Products.id=Activities.product_id))) as r1) GROUP BY name ORDER BY name  """
        conn = sqlite3.connect('moncafe.db')
        c = conn.cursor()
        c.execute(sql)
        myresult = c.fetchall()

        print("Employee Report")
        for x in myresult:
            print(x)


repo = Repository()
atexit.register(repo._close)
