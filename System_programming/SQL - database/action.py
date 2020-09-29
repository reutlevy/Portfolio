import sys

import persistene_layer
import Dto


class action:

    def create(self, activities_file):
        repo = persistene_layer.Repository()
        with open(activities_file, 'r') as f:
            for line in f:
                arguments = line.split('\n')[0]
                table = arguments.split(', ')
                product = repo.products.find(table[0])
                updatesupply = int(product.quantity) + int(table[1])
                if updatesupply >= 0:
                    product.quantity = updatesupply
                    repo.products.update(product)
                    activity = Dto.Activities(table[0], int(table[1]), table[2], table[3])
                    repo.activities.insert(activity)


def main(activities_file):
    action().create(activities_file[0])


if __name__ == '__main__':
    main(sys.argv[1:])
