/**
 * @file slib/harness.rs
 * @version 0.4.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

pub type Harness = ::Struct_harness_t;

pub type HarnessTestcase = ::Struct_harness_testcase_t;

impl Harness {

    pub fn init(&mut self)
    {
        unsafe {
            ::harness_init(self
                           as *mut _
                           as *mut ::Struct_harness_t);
        }
    }

    pub fn run(&mut self,
               testcases: &mut [HarnessTestcase])
    {
        let res;

        unsafe {
            res = ::harness_run(self
                                as *mut _
                                as *mut ::Struct_harness_t,
                                testcases
                                as *mut _
                                as *mut ::Struct_harness_testcase_t)
        }
    }
}
