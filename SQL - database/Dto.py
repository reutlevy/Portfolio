class Products:
    def __init__(self, id, description, price,quantity):
        self.id = id
        self.description = description
        self.price = price
        self.quantity = quantity


class Suppliers(object):
    def __init__(self, id, name, contact_information):
        self.contact_information = contact_information
        self.id = id
        self.name = name


class Employees(object):
    def __init__(self, id, name, salary, coffee_stand):
        self.id = id
        self.name = name
        self.salary = salary
        self.coffee_stand = coffee_stand


class Coffee_stands(object):
    def __init__(self, id, location, number_of_employees):
        self.id = id
        self.location = location
        self.number_of_employees = number_of_employees


class Activities(object):
    def __init__(self, product_id, quantity, activator_id, DATE):
        self.product_id = product_id
        self.quantity = quantity
        self.activator_id = activator_id
        self.DATE = DATE