import sqlite3
import json
import sys
import Dto
import Dao
import os
import persistene_layer
from persistene_layer import Repository as Repo


class initiate:

    def create(self, config_file):
        if os.path.isfile('moncafee.db'):
            os.remove('moncafee.db')
        open('moncafe.db', "w").close()

        repo = persistene_layer.Repository()
        repo.create_tables()

        with open(config_file, 'r') as f:
            for line in f:
                arguments = line.split('\n')[0]
                table = arguments.split(', ')
                if table[0] == 'C':
                    coffee_stand = Dto.Coffee_stands(table[1], table[2], table[3])
                    repo.coffee_stands.insert(coffee_stand)
                elif table[0] == 'S':
                    supplier = Dto.Suppliers(table[1], table[2], table[3])
                    repo.suppliers.insert(supplier)
                elif table[0] == 'E':
                    employee = Dto.Employees(table[1], table[2], table[3], table[4])
                    repo.employees.insert(employee)
                elif table[0] == 'P':
                    product = Dto.Products(table[1], table[2], table[3], 0)
                    repo.products.insert(product)


def main(config_file):
    initiate().create(config_file[0])


if __name__ == '__main__':
    main(sys.argv[1:])
