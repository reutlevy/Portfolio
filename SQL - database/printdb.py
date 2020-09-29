import Dto
import Dao
import sqlite3
import persistene_layer


class printdb:

    def run(self):
        self.printTabels()
        self.salesreport()
        self.activityreport()

    def printTabels(self):
        repo = persistene_layer.Repository()
        repo.activities.print()
        repo.coffee_stands.print()
        repo.employees.print()
        repo.products.print()
        repo.suppliers.print()

    def salesreport(self):
        repo = persistene_layer.Repository()
        repo.printsales()

    def activityreport(self):
        repo = persistene_layer.Repository()
        repo.printactivities()


def main():
    printdb().run()


if __name__ == "__main__":
    main()
