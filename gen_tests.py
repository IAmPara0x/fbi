#!/usr/bin/env python3

import random
import os
import pandas as pd
from tqdm import tqdm


if __name__ == "__main__":

    TEST_CASES=10000

    digits = [2**i for i in range(1,9)]

    tests = []

    for _ in range(TEST_CASES):
        num1 = "".join([str(random.randint(1,10)) for _ in range(random.choice(digits))])
        num2 = "".join([str(random.randint(1,10)) for _ in range(random.choice(digits))])
        result = str(int(num1) * int(num2))

        tests.append((num1,num2, result))

    df = pd.DataFrame(tests, columns=["x", "y", "result"])

    df.to_csv("tests.csv", index=False)


