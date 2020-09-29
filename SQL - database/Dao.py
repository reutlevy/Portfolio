from Dto import Employees, Suppliers, Coffee_stands, Products


class _Employees:

    def __init__(self, conn):
        self._conn = conn

    def insert(self, employee):
        self._conn.execute("""
               INSERT INTO Employees (id, name, salary, coffee_stand) VALUES (?, ?, ?, ?)
           """, [employee.id, employee.name, employee.salary, employee.coffee_stand])
        self._conn.commit()

    def find(self, Id):
        c = self._conn.cursor()
        c.execute("""
            SELECT * FROM Employees WHERE id = ?
        """, [Id])

        return Employees(*c.fetchone())

    def print(self):
        sql = "SELECT * FROM Employees ORDER BY id"
        c = self._conn.cursor()
        c.execute(sql)
        myresult = c.fetchall()

        print("Employees")
        for x in myresult:
            print(x)


class _Suppliers:

    def __init__(self, conn):
        self._conn = conn

    def insert(self, supplier):
        self._conn.execute("""
                INSERT INTO Suppliers ( id, name, contact_information) VALUES (?, ?, ?)
            """, [supplier.id, supplier.name, supplier.contact_information])
        self._conn.commit()

    def find(self,
             Id):
        c = self._conn.cursor()
        c.execute("""
            SELECT * FROM Suppliers WHERE id = ?
        """, [Id])

        return Suppliers(*c.fetchone())

    def print(self):
        sql = "SELECT * FROM Suppliers ORDER BY id"
        c = self._conn.cursor()
        c.execute(sql)
        myresult = c.fetchall()

        print("Suppliers")
        for x in myresult:
            print(x)


class _Products:

    def __init__(self, conn):
        self._conn = conn

    def insert(self, product):
        self._conn.execute("""
               INSERT INTO Products (id, description, price, quantity) VALUES (?, ?, ?, ?)
           """, [product.id, product.description, product.price, product.quantity])
        self._conn.commit()

    def find(self, product_id):
        c = self._conn.cursor()
        c.execute("""
            SELECT * FROM Products WHERE id = ?
        """, [product_id])

        return Products(*c.fetchone())

    def update(self, product):
        self._conn.execute("""
            UPDATE Products SET quantity=(?) WHERE id=(?)
        """, [product.quantity, product.id])
        self._conn.commit()

    def print(self):
        sql = "SELECT * FROM Products ORDER BY id"
        c = self._conn.cursor()
        c.execute(sql)
        myresult = c.fetchall()

        print("Products")
        for x in myresult:
            print(x)


class _Coffee_stands:

    def __init__(self, conn):
        self._conn = conn

    def insert(self, CoffeeStand):
        self._conn.execute("""
                INSERT INTO Coffee_stands (id, location, number_of_employees) VALUES (?, ?, ?)
            """, [CoffeeStand.id, CoffeeStand.location, CoffeeStand.number_of_employees])
        self._conn.commit()

    def find(self, id):
        c = self._conn.cursor()
        c.execute("""
            SELECT * FROM Coffee_stands WHERE id = ?
        """, [id])

        return Coffee_stands(*c.fetchone())

    def print(self):
        sql = "SELECT * FROM Coffee_stands ORDER BY id"
        c = self._conn.cursor()
        c.execute(sql)
        myresult = c.fetchall()

        print("Coffee_stands")
        for x in myresult:
            print(x)


class _Activities:

    def __init__(self, conn):
        self._conn = conn

    def insert(self, activity):
        self._conn.execute("""
                INSERT INTO Activities (product_id, quantity, activator_id, date) VALUES (?, ?, ?, ?)
            """, [activity.product_id, activity.quantity, activity.activator_id, activity.DATE])
        self._conn.commit()

    def print(self):
        sql = "SELECT * FROM Activities ORDER BY date"
        c = self._conn.cursor()
        c.execute(sql)
        myresult = c.fetchall()

        print("Activities")
        for x in myresult:
            print(x)
