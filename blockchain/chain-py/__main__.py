from .block import Block
from .chain import BlockChain

"""
Thanks to Alfrick for the basics.
https://github.com/Alfrick/Create-Cryptocurrency-in-Python/blob/master/crypto.py
"""



def main():

    blockchain = BlockChain()

    print("Mining is about to start")
    print(blockchain.chain)

    last_block = blockchain.latest_block
    last_proof_no = last_block.proof_no
    proof_no = blockchain.proof_of_work(last_proof_no)

    blockchain.new_data(sender="0", recipient="Axel", quantity= 1)

    last_hash = last_block.calculate_hash
    block = blockchain.construct_block(proof_no, last_hash)

    print("Mining has been successful")
    print(blockchain.chain)


if __name__ == "__main__":
    main()
