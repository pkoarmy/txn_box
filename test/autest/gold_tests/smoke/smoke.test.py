# @file
#
# Copyright 2020, Verizon Media
# SPDX-License-Identifier: Apache-2.0
#
'''
Basic smoke tests.
'''
Test.Summary = '''
Test basic functions and directives.
'''

#Test.TxnBoxTest("smoke.replay.yaml", config_key="meta.txn_box"
#                ,remap=[('http://example.one', 'http://example.one')]
#                )
#Test.TxnBoxRun("Smoke test")

Test.TxnBoxTestAndRun("Smoke Test", "smoke.replay.yaml", config_path='Auto', config_key="meta.txn_box"
                ,remap=[('http://example.one', 'http://example.one')]
                )